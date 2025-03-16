#include <fmb.h>
#include <common.h>

FMBDataType num_to_data_type(int num)
{
    switch (num)
    {
        case 5120: return FMB_BYTE;
		case 5121: return FMB_UNSIGNED_BYTE;
		case 5122: return FMB_SHORT;
		case 5123: return FMB_UNSIGNED_SHORT;

		default: return FMB_FLOAT;
    }
}

int data_size(FMBDataType dataType)
{
    switch (dataType)
    {
        case FMB_BYTE:  case FMB_UNSIGNED_BYTE:   return 1;
        case FMB_SHORT: case FMB_UNSIGNED_SHORT:  return 2;
        
        default: return 4;
    }
}

FMB* fmb_load(FILE* file)
{
    FMB* fmb = (FMB*)malloc(sizeof(FMB));
    FREADS(fmb->version);

    FMBGLDataHeader* glData = (FMBGLDataHeader*)malloc(sizeof(FMBGLDataHeader));
    FREAD(glData, sizeof(FMBGLDataHeader));

    #define NEXT_TYPE(s) fmb->s##DataType = num_to_data_type(glData->s);\
    fmb->s##DataSize = data_size(fmb->s##DataType)

    NEXT_TYPE(index  );
    NEXT_TYPE(vertex );
    NEXT_TYPE(normal );
    NEXT_TYPE(texture);
    NEXT_TYPE(color  );

    DEBUGF("index type %i\n", fmb->indexDataType);
    DEBUGF("vertex type %i\n", fmb->vertexDataType);
    DEBUGF("normal type %i\n", fmb->normalDataType);
    DEBUGF("texture type %i\n", fmb->textureDataType);
    DEBUGF("color type %i\n", fmb->colorDataType);

    #undef NEXT_TYPE

    free(glData);

    FREADS(fmb->offset);
    FREADS(fmb->scale);
    FREADS(fmb->numFrames);
    FREADS(fmb->numMaterials);

    fmb->inverseScale = 1.0f / fmb->scale;

    CALLOC(fmb->materials, fmb->numMaterials, FMBMaterial);

    for (int i = 0; i < fmb->numMaterials; i++)
    {
        fmb->materials[i].name = freadnts(file);
        fmb->materials[i].texturePath = freadnts(file);

        FREADS(fmb->materials[i].ambient);
        FREADS(fmb->materials[i].diffuse);
        FREADS(fmb->materials[i].specular);
        FREADS(fmb->materials[i].glossiness);
    }

    FREADS(fmb->numObjects);
    CALLOC(fmb->objects, fmb->numObjects, FMBObject);

    for (int i = 0; i < fmb->numObjects; i++)
    {
        fmb->objects[i].name = freadnts(file);
        
        FREADS(fmb->objects[i].materialIndex);

        FREADS(fmb->objects[i].hasNormals);
        FREADS(fmb->objects[i].hasTextures);
        FREADS(fmb->objects[i].hasColors);

        FREADS(fmb->objects[i].numKeyFrames);

        FREADP(fmb->objects[i].frames, sizeof(FMBFrame), fmb->objects[i].numKeyFrames);

        FREADS(fmb->objects[i].numFaces);
        FREADA(fmb->objects[i].indices, fmb->objects[i].numFaces * fmb->indexDataSize * 3);
        
        FMBFrame lastFrame = fmb->objects[i].frames[fmb->objects[i].numKeyFrames - 1];
        FREADS(fmb->objects[i].numVertices);

        int vertexCount = fmb->objects[i].numVertices * 3 * (lastFrame.verticesOffset + 1);

        FREADA(fmb->objects[i].vertices, vertexCount * fmb->vertexDataSize);

        fpos_t pos;
        fgetpos(file, &pos);

        if (fmb->objects[i].hasNormals)
        {
            FREADA(fmb->objects[i].normals, vertexCount * fmb->normalDataSize);
        }

        if (fmb->objects[i].hasTextures)
        {
            FREADA(fmb->objects[i].textures, fmb->objects[i].numVertices * 2 * fmb->textureDataSize);
        }

        if (fmb->objects[i].hasColors)
        {
            FREADA(fmb->objects[i].colors, fmb->objects[i].numVertices * 4 * fmb->colorDataSize);
        }

        FREADP(fmb->objects[i].centers, sizeof(Vec3), fmb->objects[i].numKeyFrames);
        FREADP(fmb->objects[i].radiuses, sizeof(float), fmb->objects[i].numKeyFrames);

        FREADP(fmb->objects[i].keyFrameLookUp, sizeof(uint16_t), (fmb->numFrames + 1));
    }

    FREADP(fmb->mins, sizeof(Vec3), fmb->numFrames);
    FREADP(fmb->maxes, sizeof(Vec3), fmb->numFrames);

    return fmb;
}

void fmb_delete(FMB* fmb)
{
    if (fmb->numMaterials > 0)
    {
        for (int i = 0; i < fmb->numMaterials; i++)
        {
            FREE(fmb->materials[i].name);
            FREE(fmb->materials[i].texturePath);
        }

        FREE(fmb->materials);
    }

    if (fmb->numObjects > 0)
    {
        for (int i = 0; i < fmb->numObjects; i++)
        {
            FREE(fmb->objects[i].name);
            FREE(fmb->objects[i].frames);

            FREE(fmb->objects[i].indices);
            FREE(fmb->objects[i].vertices);
            FREE(fmb->objects[i].normals);
            FREE(fmb->objects[i].textures);
            FREE(fmb->objects[i].colors);

            FREE(fmb->objects[i].centers);
            FREE(fmb->objects[i].radiuses);
            FREE(fmb->objects[i].keyFrameLookUp);
        }

        FREE(fmb->objects);
    }

    FREE(fmb->mins);
    FREE(fmb->maxes);

    FREE(fmb);
}