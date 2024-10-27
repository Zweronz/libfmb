#include <fmb2.h>

#define CHUNK_BUF_SIZE 10   //the chunk buffer isn't based on chunk count, it's apparently just a fixed value that fills with empty entries past the chunk count
#define CHUNK_SIZE 8        //int32*2 (4 char label, chunk len)

void fmb2_read_data(Stream* stream, FMB2* fmb2, size_t length)
{
    fmb2->data.offset       = stream_float(stream);
    fmb2->data.scale        = stream_float(stream);
    fmb2->data.numMaterials = stream_int(stream);
    fmb2->data.numObjects   = stream_int(stream);
    fmb2->data.numFrames    = stream_int(stream);
    fmb2->data.numKeyFrames = stream_int(stream);

    fmb2->data.objects = CALLOC(FMB2Object, fmb2->data.numObjects);

    FOREACH (i, fmb2->data.numObjects)
    {
        FMB2Object* object = &(fmb2->data.objects[i]);

        object->name = stream_string(stream);

        object->materialIndex     = stream_int(stream);
        object->numFaces          = stream_int(stream);
        object->numVertices       = stream_int(stream);
        object->numKeyFrames      = stream_int(stream);
        object->drawDataSize      = stream_int(stream);
        object->numVertexChannels = stream_int(stream);

        object->vertexChannels = CALLOC(FMB2VertexChannel, object->numVertexChannels);

        FOREACH (j, object->numVertexChannels)
        {
            FMB2VertexChannel* channel = &(object->vertexChannels[j]);

            channel->exportedType = (FMB2VertexChannelType)stream_int(stream);

            channel->dataType   = stream_int(stream);
            channel->dataSize   = stream_int(stream);
            channel->components = stream_int(stream);
            channel->offsets    = stream_int(stream);
        }
    }
}

void fmb2_read_anim(Stream* stream, FMB2* fmb2, size_t length)
{
    fmb2->anim.keyFrameLookUp = (unsigned short*)stream_data(stream, length);
}

void fmb2_read_bnds(Stream* stream, FMB2* fmb2, size_t length)
{
    fmb2->bnds.bounds = CALLOC(FMB2ObjectBounds, fmb2->data.numObjects);

    FOREACH (i, fmb2->data.numObjects)
    {
        fmb2->bnds.bounds[i].numFrames = stream_int(stream);
    }

    FOREACH (i, fmb2->data.numObjects)
    {
        fmb2->bnds.bounds[i].frameLookUp = STREAM_ARR(unsigned short, fmb2->data.numFrames);
    }

    FOREACH (i, fmb2->data.numObjects)
    {
        fmb2->bnds.bounds[i].radiuses = CALLOC(float, fmb2->bnds.bounds[i].numFrames);
        fmb2->bnds.bounds[i].mins     = CALLOC(Vec3,  fmb2->bnds.bounds[i].numFrames);
        fmb2->bnds.bounds[i].maxes    = CALLOC(Vec3,  fmb2->bnds.bounds[i].numFrames);
        fmb2->bnds.bounds[i].centers  = CALLOC(Vec3,  fmb2->bnds.bounds[i].numFrames);

        FOREACH (j, fmb2->bnds.bounds[i].numFrames)
        {
            fmb2->bnds.bounds[i].radiuses[i] = stream_float(stream);
            
            Vec3* center = STREAM_DATA(Vec3), *min = STREAM_DATA(Vec3), *max = STREAM_DATA(Vec3);

            fmb2->bnds.bounds[i].centers[i] = *center;
            fmb2->bnds.bounds[i].mins[i]    = *min;
            fmb2->bnds.bounds[i].maxes[i]   = *max;

            free(min);
            free(max);
            free(center);
        }
    }
}

void fmb2_read_dums(Stream* stream, FMB2* fmb2, size_t length)
{
    fmb2->dums.numDummies = stream_int(stream);

    fmb2->dums.dummies = CALLOC(FMB2Dummy, fmb2->dums.numDummies);

    FOREACH (i, fmb2->dums.numDummies)
    {
        fmb2->dums.dummies[i].name = stream_string(stream);
    }

    FOREACH (i, fmb2->dums.numDummies)
    {
        fmb2->dums.dummies[i].positions = CALLOC(Vec3, fmb2->data.numFrames);
        fmb2->dums.dummies[i].rotations = CALLOC(Vec3, fmb2->data.numFrames);

        FOREACH (j, fmb2->data.numFrames)
        {
            Vec3* position = STREAM_DATA(Vec3), *rotation = STREAM_DATA(Vec3);
            
            fmb2->dums.dummies[i].positions[j] = *position;
            fmb2->dums.dummies[i].rotations[j] = *rotation;

            free(position);
            free(rotation);
        }
    }
}

void fmb2_read_face(Stream* stream, FMB2* fmb2, size_t length)
{
    fmb2->face.faces = CALLOC(FMB2ObjectFaces, fmb2->data.numObjects);

    FOREACH (i, fmb2->data.numObjects)
    {
        fmb2->face.faces[i].indices = STREAM_ARR(unsigned short, fmb2->data.objects[i].numFaces);
    }
}

void fmb2_read_chnd(Stream* stream, FMB2* fmb2, size_t length)
{
    fmb2->chnd.data = CALLOC(FMB2VertexData, fmb2->data.numObjects);

    FOREACH (i, fmb2->data.numObjects)
    {
        FMB2Object* object = &(fmb2->data.objects[i]);
        FMB2VertexData* vertexData = &(fmb2->chnd.data[i]);

        vertexData->channels = CALLOC(FMB2ObjectChannel, object->numVertexChannels);

        FOREACH (j, object->numVertexChannels)
        {
            vertexData->channels[j].vertexData  = STREAM_ARR(char, object->numVertices * object->vertexChannels[j].dataSize * object->vertexChannels[j].components * object->vertexChannels[j].offsets);
            vertexData->channels[j].frameLookUp = STREAM_ARR(unsigned short, fmb2->data.numFrames);
        }
    }
}

void fmb2_read_bone(Stream* stream, FMB2* fmb2, size_t length)
{
    ADVANCE(length);
}

FMB2* fmb2_from_stream(Stream* stream)
{
    //skip null terminator not read by loader (implement this better later!!)
    ADVANCE(1);

    FMB2* fmb2 = (FMB2*)malloc(sizeof(FMB2));
    
    fmb2->version    = stream_float(stream);
    fmb2->chunkCount = stream_int(stream);

    printf("loading fmb2 v%g\n", fmb2->version);

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
        printf("fmb2 chunk count surpasses the chunk buffer (10), unexpected things may happen!\n");
    }

    #define CASE(t, tl) case t: fmb2_read_##tl(stream, fmb2, chunks[i].length); printf(#tl " %zu %i\n", stream->pos, i); break

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

    return fmb2;
}

void fmb2_delete(FMB2* fmb2)
{
    printf("do it later bro");
}