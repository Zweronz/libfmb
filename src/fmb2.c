#include <fmb2.h>

#define CHUNK_BUF_SIZE 10   //the chunk buffer isn't based on chunk count, it's apparently just a fixed value that fills with empty entries past the chunk count
#define CHUNK_SIZE 8        //int32*2 (4 char label, chunk len)

void fmb2_old_from_stream(FMB2* fmb2, Stream* stream)
{
    fmb2->offset       = stream_float(stream);
    fmb2->scale        = stream_float(stream);

    fmb2->numKeyFrames = stream_int(stream);
    fmb2->numFrames    = stream_int(stream);

    fmb2->frameToKeyFrame       = STREAM_ARR(unsigned short, fmb2->numFrames);
    fmb2->keyFrameToFrameNumber = STREAM_ARR(unsigned short, fmb2->numKeyFrames);

    fmb2->numMaterials = stream_int(stream);
    fmb2->numModels    = stream_int(stream);

    fmb2->models = CALLOC(FMB2Model, fmb2->numModels);

    FOREACH (i, fmb2->numModels)
    {
        fmb2->models[i].name          = stream_string(stream);

        fmb2->models[i].faceGeomType  = stream_int(stream);
        fmb2->models[i].numFaces      = stream_int(stream);

        fmb2->models[i].numVertices   = stream_int(stream);

        fmb2->models[i].indexDataType = stream_int(stream);
        fmb2->models[i].indexDataSize = stream_int(stream);

        fmb2->models[i].indices       = (char*)stream_data(stream, fmb2->models[i].numFaces * fmb2->models[i].indexDataSize * 3);
        
        fmb2->models[i].numChannels   = stream_int(stream);

        fmb2->models[i].channels = CALLOC(FMB2VertexChannel, fmb2->models[i].numChannels);

        FOREACH (j, fmb2->models[i].numChannels)
        {
            fmb2->models[i].channels[j].exportedType  = (FMB2VertexChannelType)stream_int(stream);

            fmb2->models[i].channels[j].dataType      = stream_int(stream);
            fmb2->models[i].channels[j].dataSize      = stream_int(stream);

            fmb2->models[i].channels[j].numComponents = stream_int(stream);
            fmb2->models[i].channels[j].numOffsets    = stream_int(stream);

            fmb2->models[i].channels[j].data = (char*)stream_data(stream, fmb2->models[i].numVertices * fmb2->models[i].channels[j].dataSize * fmb2->models[i].channels[j].numComponents * fmb2->models[i].channels[j].numOffsets);
            fmb2->models[i].channels[j].keyFrameToOffset = STREAM_ARR(unsigned short, fmb2->numKeyFrames);
        }

        fmb2->models[i].boundingSpheres = STREAM_ARR(Vec4, fmb2->numKeyFrames);
        fmb2->models[i].mins            = STREAM_ARR(Vec3, fmb2->numKeyFrames);
        fmb2->models[i].maxes           = STREAM_ARR(Vec3, fmb2->numKeyFrames);
    }


    if (fmb2->version >= 1.01f)
    {
        fmb2->numDummies = stream_int(stream);
        fmb2->dummies = CALLOC(FMB2Dummy, fmb2->numDummies);

        FOREACH (i, fmb2->numDummies)
        {
            fmb2->dummies[i].name = stream_string(stream);
            fmb2->dummies[i].frameData = (char*)stream_data(stream, 24 * fmb2->numKeyFrames); //24 = Vec3 pos, Vec3 rot
        }
    }

    //4 bytes of padding (00 00 00 00) at the bottom of the file??
}

void fmb2_read_data(Stream* stream, FMB2* fmb2, size_t length)
{
    fmb2->offset       = stream_float(stream);
    fmb2->scale        = stream_float(stream);
    fmb2->numMaterials = stream_int(stream);
    fmb2->numModels    = stream_int(stream);
    fmb2->numKeyFrames = stream_int(stream);
    fmb2->numFrames    = stream_int(stream);

    fmb2->models = CALLOC(FMB2Model, fmb2->numModels);

    FOREACH (i, fmb2->numModels)
    {
        FMB2Model* model = &(fmb2->models[i]);

        model->name = stream_string(stream);

        model->materialIndex = stream_int(stream);
        model->numFaces      = stream_int(stream);
        model->numVertices   = stream_int(stream);
        model->numKeyFrames  = stream_int(stream);
        model->indexDataSize = stream_int(stream);
        model->numChannels   = stream_int(stream);

        model->channels = CALLOC(FMB2VertexChannel, model->numChannels);

        FOREACH (j, model->numChannels)
        {
            model->channels[j].exportedType = (FMB2VertexChannelType)stream_int(stream);

            model->channels[j].dataType      = stream_int(stream);
            model->channels[j].dataSize      = stream_int(stream);
            model->channels[j].numComponents = stream_int(stream);
            model->channels[j].numOffsets    = stream_int(stream);
        }
    }
}

void fmb2_read_anim(Stream* stream, FMB2* fmb2, size_t length)
{
    fmb2->frameToKeyFrame       = STREAM_ARR(unsigned short, fmb2->numFrames);
    fmb2->keyFrameToFrameNumber = STREAM_ARR(unsigned short, fmb2->numKeyFrames);
}

void fmb2_read_bnds(Stream* stream, FMB2* fmb2, size_t length)
{
    FOREACH (i, fmb2->numModels)
    {
        fmb2->models[i].numBoundingOffsets = stream_int(stream);
    }

    FOREACH (i, fmb2->numModels)
    {
        fmb2->models[i].boundingOffsetToKeyFrame = STREAM_ARR(unsigned short, fmb2->numKeyFrames);
    }

    FOREACH (i, fmb2->numModels)
    {
        //idk if this is right, they might be interleaved
        fmb2->models[i].boundingSpheres = STREAM_ARR(Vec4, fmb2->models[i].numBoundingOffsets);
        fmb2->models[i].mins            = STREAM_ARR(Vec3, fmb2->models[i].numBoundingOffsets);
        fmb2->models[i].maxes           = STREAM_ARR(Vec3, fmb2->models[i].numBoundingOffsets);
    }
}

void fmb2_read_dums(Stream* stream, FMB2* fmb2, size_t length)
{
    fmb2->numDummies = stream_int(stream);
    fmb2->dummies    = CALLOC(FMB2Dummy, fmb2->numDummies);

    FOREACH (i, fmb2->numDummies)
    {
        fmb2->dummies[i].name = stream_string(stream);
    }

    FOREACH (i, fmb2->numDummies)
    {
        fmb2->dummies[i].frameData = (char*)stream_data(stream, 24 * fmb2->numKeyFrames);
    }
}

void fmb2_read_face(Stream* stream, FMB2* fmb2, size_t length)
{
    FOREACH (i, fmb2->numModels)
    {
        fmb2->models[i].indices = STREAM_ARR(char, fmb2->models[i].numFaces * fmb2->models[i].indexDataSize * 3);
    }
}

void fmb2_read_chnd(Stream* stream, FMB2* fmb2, size_t length)
{
    FOREACH (i, fmb2->numModels)
    {
        FOREACH (j, fmb2->models[i].numChannels)
        {
            fmb2->models[i].channels[j].data = STREAM_ARR(char, fmb2->models[i].numVertices * fmb2->models[i].channels[j].dataSize * fmb2->models[i].channels[j].numComponents * fmb2->models[i].channels[j].numOffsets);
            fmb2->models[i].channels[j].keyFrameToOffset = STREAM_ARR(unsigned short, fmb2->numKeyFrames);
        }
    }
}

void fmb2_read_bone(Stream* stream, FMB2* fmb2, size_t length)
{
    ADVANCE(length);
}

FMB2* fmb2_from_stream(Stream* stream)
{
    FMB2* fmb2 = (FMB2*)malloc(sizeof(FMB2));
    fmb2->version = stream_float(stream);

    if (fmb2->version < 1.03f)
    {
        fmb2_old_from_stream(fmb2, stream);
    }
    else
    {
        fmb2->chunkCount = stream_int(stream);

        FMB2Chunk* chunks = CALLOC(FMB2Chunk, fmb2->chunkCount);

        FOREACH (i, fmb2->chunkCount)
        {
            chunks[i].label  = stream_int(stream);
            chunks[i].length = stream_int(stream);
        }

        //skip the remaining unused chunk buffers
        if (fmb2->chunkCount < 10)
        {
            ADVANCE((CHUNK_BUF_SIZE - fmb2->chunkCount) * CHUNK_SIZE);
        }
        else if (fmb2->chunkCount > 10)
        {
            debug("fmb2 chunk count surpasses the chunk buffer (10), unexpected things may happen!\n");
        }

        #define CASE(t, tl) case t: fmb2_read_##tl(stream, fmb2, chunks[i].length); break

        FOREACH (i, fmb2->chunkCount)
        {
            switch (chunks[i].label)
            {
                CASE(DATA, data);
                CASE(ANIM, anim);
                CASE(BNDS, bnds);
                CASE(DUMS, dums);
                CASE(FACE, face);
                CASE(CHND, chnd);
                CASE(BONE, bone);
            }
        }

        #undef CASE

        FREE(chunks); 
    }

    return fmb2;
}

void fmb2_delete(FMB2* fmb2)
{
    if (fmb2 != NULL)
    {
        FREE(fmb2->frameToKeyFrame);
        FREE(fmb2->keyFrameToFrameNumber);

        FOREACH (i, fmb2->numModels)
        {
            FREE(fmb2->models[i].name);
            FREE(fmb2->models[i].indices);

            FOREACH (j, fmb2->models[i].numChannels)
            {
                FREE(fmb2->models[i].channels[j].data);
                FREE(fmb2->models[i].channels[j].keyFrameToOffset);
            }

            FREE(fmb2->models[i].channels);

            FREE(fmb2->models[i].boundingSpheres);
            FREE(fmb2->models[i].mins);
            FREE(fmb2->models[i].maxes);

            FREE(fmb2->models[i].boundingOffsetToKeyFrame);
        }

        FREE(fmb2->models);

        if (fmb2->version > 1.01f)
        {
            FOREACH (i, fmb2->numDummies)
            {
                FREE(fmb2->dummies[i].name);
                FREE(fmb2->dummies[i].frameData);
            }

            FREE(fmb2->dummies);
        }

        free(fmb2);
    }
}