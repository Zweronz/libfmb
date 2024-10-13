#include <fmb2.h>

#define CHUNK_BUF_SIZE 10   //the chunk buffer isn't based on chunk count, it's apparently just a fixed value that fills with empty entries past the chunk count
#define CHUNK_SIZE 8        //int32*2 (4 char label, chunk len)
#define HEADER_SIZE 5       //fmb2\0

void fmb2_read_data(Stream* stream, FMB2* fmb2, size_t length)
{
    fmb2->data.offset = stream_float(stream);
    fmb2->data.scale = stream_float(stream);

    fmb2->data.numMaterials = stream_int(stream);
    fmb2->data.numObjects = stream_int(stream);

    fmb2->data.numFrames = stream_int(stream);
    fmb2->data.numKeyFrames = stream_int(stream);

    fmb2->data.objects = (FMB2Object*)calloc(fmb2->data.numObjects, sizeof(FMB2Object));

    FOR (fmb2->data.numObjects)
    {
        FMB2Object* object = &(fmb2->data.objects[i]);

        object->name = stream_string(stream);

        object->materialIndex = stream_int(stream);
        object->numFaces = stream_int(stream);
        
        object->numVertices = stream_int(stream);
        object->numKeyFrames = stream_int(stream);

        object->drawDataSize = stream_int(stream);
        object->numVertexChannels = stream_int(stream);

        object->vertexChannels = (FMB2VertexChannel*)calloc(object->numVertexChannels, sizeof(FMB2VertexChannel));

        FOR_N (j, object->numVertexChannels)
        {
            FMB2VertexChannel* channel = &(object->vertexChannels[j]);

            channel->exportedType = (FMB2VertexChannelType)stream_int(stream);

            channel->dataType = stream_int(stream);
            channel->dataSize = stream_int(stream);
            channel->components = stream_int(stream);
            channel->offsets = stream_int(stream);
        }
    }
}

void fmb2_read_anim(Stream* stream, FMB2* fmb2, size_t length)
{
    fmb2->anim.keyFrameLookUp = (unsigned short*)stream_data(stream, length);
}

void fmb2_read_bnds(Stream* stream, FMB2* fmb2, size_t length)
{
    ADVANCE(length);
}

void fmb2_read_dums(Stream* stream, FMB2* fmb2, size_t length)
{
    fmb2->dums.numDummies = stream_int(stream);

    fmb2->dums.dummies = (FMB2Dummy*)calloc(fmb2->dums.numDummies, sizeof(FMB2Dummy));

    FOR (fmb2->dums.numDummies)
    {
        fmb2->dums.dummies[i].name = stream_string(stream);
    }

    FOR (fmb2->dums.numDummies)
    {
        fmb2->dums.dummies[i].positions = (Vec3*)calloc(fmb2->data.numFrames, sizeof(Vec3));
        fmb2->dums.dummies[i].rotations = (Vec3*)calloc(fmb2->data.numFrames, sizeof(Vec3));

        FOR_N (j, fmb2->data.numFrames)
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
    fmb2->face.indices = (unsigned short*)stream_data(stream, length);
}

void fmb2_read_chnd(Stream* stream, FMB2* fmb2, size_t length)
{
    fmb2->chnd.data = (FMB2VertexData*)calloc(fmb2->data.numObjects, sizeof(FMB2VertexData));

    FOR (fmb2->data.numObjects)
    {
        FMB2Object* object = &(fmb2->data.objects[i]);
        FMB2VertexData* vertexData = &(fmb2->chnd.data[i]);

        vertexData->channels = (FMB2ObjectChannel*)calloc(object->numVertexChannels, sizeof(FMB2ObjectChannel));

        FOR_N (j, object->numVertexChannels)
        {
            vertexData->channels[j].vertexData = STREAM_ARR(char, object->numVertices * object->vertexChannels[j].dataSize * object->vertexChannels[j].components * object->vertexChannels[j].offsets);
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
    ADVANCE(HEADER_SIZE);

    FMB2* fmb2 = (FMB2*)malloc(sizeof(FMB2));
    
    fmb2->version = stream_float(stream);
    fmb2->chunkCount = stream_int(stream);

    printf("loading fmb2 v%g\n", fmb2->version);

    FMB2Chunk* chunks = (FMB2Chunk*)calloc(fmb2->chunkCount, sizeof(FMB2Chunk));

    FOR (fmb2->chunkCount)
    {
        chunks[i].label = stream_int(stream);
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

    FOR (fmb2->chunkCount)
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

    FREE(chunks);

    return fmb2;
}

void fmb2_delete(FMB2* fmb2)
{
    printf("do it later bro");
}