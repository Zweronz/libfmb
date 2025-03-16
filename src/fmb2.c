#include <fmb2.h>

#define CHUNK_BUF_SIZE 10   //the chunk buffer isn't based on chunk count, it's apparently just a fixed value that fills with empty entries past the chunk count

//pre-chunk versions ranging 1.0-1.0.2
void fmb2_old_load(FMB2* fmb2, FILE* file)
{
    FREADS(fmb2->offset);
    FREADS(fmb2->scale);
    FREADS(fmb2->numKeyFrames);
    FREADS(fmb2->numFrames);

    FREADP(fmb2->frameToKeyFrame,       sizeof(uint16_t), fmb2->numFrames);
    FREADP(fmb2->keyFrameToFrameNumber, sizeof(uint16_t), fmb2->numKeyFrames);
    
    FREADS(fmb2->numMaterials);
    FREADS(fmb2->numModels);

    CALLOC(fmb2->models, fmb2->numModels, FMB2Model);

    for (int i = 0; i < fmb2->numModels; i++)
    {
        fmb2->models[i].name = freadnts(file);

        FREADS(fmb2->models[i].materialIndex);
        FREADS(fmb2->models[i].numFaces);
        FREADS(fmb2->models[i].numVertices);
        FREADS(fmb2->models[i].indexDataType);
        FREADS(fmb2->models[i].indexDataSize);

        FREADP(fmb2->models[i].indices, fmb2->models[i].indexDataSize * 3, fmb2->models[i].numFaces);
        FREADS(fmb2->models[i].numChannels);

        CALLOC(fmb2->models[i].channels, fmb2->models[i].numChannels, FMB2VertexChannel);
        fmb2->models[i].numBoundingOffsets = 0;

        for (int j = 0; j < fmb2->models[i].numChannels; j++)
        {
            FREADS(fmb2->models[i].channels[j].exportedType);
            FREADS(fmb2->models[i].channels[j].dataType);
            FREADS(fmb2->models[i].channels[j].dataSize);
            FREADS(fmb2->models[i].channels[j].numComponents);
            FREADS(fmb2->models[i].channels[j].numOffsets);

            FREADA(fmb2->models[i].channels[j].data, fmb2->models[i].numVertices * fmb2->models[i].channels[j].dataSize * fmb2->models[i].channels[j].numComponents * fmb2->models[i].channels[j].numOffsets);
            FREADP(fmb2->models[i].channels[j].keyFrameToOffset, sizeof(uint16_t), fmb2->numKeyFrames);

            if (fmb2->models[i].channels[j].exportedType == Position)
            {
                fmb2->models[i].numBoundingOffsets = fmb2->models[i].channels[j].numOffsets;
            }
        }

        FREADP(fmb2->models[i].boundingSpheres, sizeof(Vec4), fmb2->models[i].numBoundingOffsets);
        FREADP(fmb2->models[i].mins,            sizeof(Vec3), fmb2->models[i].numBoundingOffsets);
        FREADP(fmb2->models[i].maxes,           sizeof(Vec3), fmb2->models[i].numBoundingOffsets);
    }


    if (fmb2->version >= 1.01f)
    {
        FREADS(fmb2->numDummies);
        CALLOC(fmb2->dummies, fmb2->numDummies, FMB2Dummy);

        for (int i = 0; i < fmb2->numDummies; i++)
        {
            fmb2->dummies[i].name = freadnts(file);

            if (fmb2->version >= 1.02f)
            {
                FREADP(fmb2->dummies[i].frameData, 24, fmb2->numKeyFrames); //24 = Vec3 pos, Vec3 rot
            }
            else
            {
                FREADP(fmb2->dummies[i].frameData, sizeof(Vec3), fmb2->numKeyFrames);
            }
        }
    }

    //4 bytes of padding (00 00 00 00) at the bottom of the file?? (sometimes)
}

void fmb2_read_data(FILE* file, FMB2* fmb2, int32_t length)
{
    FREADS(fmb2->offset);
    FREADS(fmb2->scale);

    FREADS(fmb2->numMaterials);
    FREADS(fmb2->numModels);

    FREADS(fmb2->numKeyFrames);
    FREADS(fmb2->numFrames);

    CALLOC(fmb2->models, fmb2->numModels, FMB2Model);

    for (int i = 0; i < fmb2->numModels; i++)
    {
        fmb2->models[i].name = freadnts(file);
        
        FREADS(fmb2->models[i].materialIndex);
        FREADS(fmb2->models[i].numFaces     );
        FREADS(fmb2->models[i].numVertices  );

        FREADS(fmb2->models[i].numKeyFrames );
        FREADS(fmb2->models[i].indexDataSize);
        FREADS(fmb2->models[i].numChannels  );

        CALLOC(fmb2->models[i].channels, fmb2->models[i].numChannels, FMB2VertexChannel);

        for (int j = 0; j < fmb2->models[i].numChannels; j++)
        {
            FREADS(fmb2->models[i].channels[j].exportedType);
            FREADS(fmb2->models[i].channels[j].dataType);
            FREADS(fmb2->models[i].channels[j].dataSize);
            FREADS(fmb2->models[i].channels[j].numComponents);
            FREADS(fmb2->models[i].channels[j].numOffsets);
        }
    }
}

void fmb2_read_anim(FILE* file, FMB2* fmb2, int32_t length)
{
    FREADP(fmb2->frameToKeyFrame,       sizeof(uint16_t), fmb2->numFrames);
    FREADP(fmb2->keyFrameToFrameNumber, sizeof(uint16_t), fmb2->numKeyFrames);
}

void fmb2_read_bnds(FILE* file, FMB2* fmb2, int32_t length)
{
    for (int i = 0; i < fmb2->numModels; i++)
    {
        FREADS(fmb2->models[i].numBoundingOffsets);
    }

    for (int i = 0; i < fmb2->numModels; i++)
    {
        FREADP(fmb2->models[i].boundingOffsetToKeyFrame, sizeof(uint16_t), fmb2->numKeyFrames);
    }

    for (int i = 0; i < fmb2->numModels; i++)
    {
        //idk if this is right, they might be interleaved
        FREADP(fmb2->models[i].boundingSpheres, sizeof(Vec4), fmb2->models[i].numBoundingOffsets);
        FREADP(fmb2->models[i].mins,            sizeof(Vec3), fmb2->models[i].numBoundingOffsets);
        FREADP(fmb2->models[i].maxes,           sizeof(Vec3), fmb2->models[i].numBoundingOffsets);
    }
}

void fmb2_read_dums(FILE* file, FMB2* fmb2, int32_t length)
{
    FREADS(fmb2->numDummies);
    CALLOC(fmb2->dummies, fmb2->numDummies, FMB2Dummy);

    for (int i = 0; i < fmb2->numDummies; i++)
    {
        fmb2->dummies[i].name = freadnts(file);
    }

    for (int i = 0; i < fmb2->numDummies; i++)
    {
        FREADP(fmb2->dummies[i].frameData, 24, fmb2->numKeyFrames); //24 = Vec3 pos, Vec3 rot
    }
}

void fmb2_read_face(FILE* file, FMB2* fmb2, int32_t length)
{
    for (int i = 0; i < fmb2->numModels; i++)
    {
        FREADP(fmb2->models[i].indices, fmb2->models[i].indexDataSize * 3, fmb2->models[i].numFaces);
    }
}

void fmb2_read_chnd(FILE* file, FMB2* fmb2, int32_t length)
{
    for (int i = 0; i < fmb2->numModels; i++)
    {
        for (int j = 0; j < fmb2->models[i].numChannels; j++)
        {
            FREADA(fmb2->models[i].channels[j].data, fmb2->models[i].numVertices * fmb2->models[i].channels[j].dataSize * fmb2->models[i].channels[j].numComponents * fmb2->models[i].channels[j].numOffsets);
            FREADP(fmb2->models[i].channels[j].keyFrameToOffset, sizeof(uint16_t), fmb2->numKeyFrames);
        }
    }
}

void fmb2_read_bone(FILE* file, FMB2* fmb2, int32_t length)
{
    fadvance((size_t)length, file);
}

FMB2* fmb2_load(FILE* file)
{
    FMB2* fmb2 = (FMB2*)malloc(sizeof(FMB2));
    FREADS(fmb2->version);

    if (fmb2->version < 1.03f)
    {
        fmb2_old_load(fmb2, file);
    }
    //post-chunk versions ranging 1.0.3-1.1
    else
    {
        FREADS(fmb2->numChunks);

        FMB2Chunk* chunks;
        FREADP(chunks, sizeof(FMB2Chunk), fmb2->numChunks);

        //skip the remaining unused chunk buffers
        if (fmb2->numChunks < 10)
        {
            fadvance((CHUNK_BUF_SIZE - fmb2->numChunks) * sizeof(FMB2Chunk), file);
        }
        else if (fmb2->numChunks > 10)
        {
            DEBUG("fmb2 chunk count surpasses the chunk buffer (10), unexpected things may happen!\n");
        }

        #define CASE(t, tl) case t: fmb2_read_##tl(file, fmb2, chunks[i].length); break

        for (int i = 0; i < fmb2->numChunks; i++)
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
    FREE(fmb2->frameToKeyFrame);
    FREE(fmb2->keyFrameToFrameNumber);

    for (int i = 0; i < fmb2->numModels; i++)
    {
        FREE(fmb2->models[i].name);
        FREE(fmb2->models[i].indices);

        for (int j = 0; j < fmb2->models[i].numChannels; j++)
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
        for (int i = 0; i < fmb2->numDummies; i++)
        {
            FREE(fmb2->dummies[i].name);
            FREE(fmb2->dummies[i].frameData);
        }

        FREE(fmb2->dummies);
    }

    FREE(fmb2);
}