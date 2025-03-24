#include <umb.h>
#include <malloc.h>
#include <common.h>

UMB* umb_load(FILE* file)
{
    UMB* umb = (UMB*)malloc(sizeof(UMB));
    FREADS(umb->numMaterials);

    CALLOC(umb->materials, umb->numMaterials, UMBMaterial);

    for (int i = 0; i < umb->numMaterials; i++)
    {
        umb->materials[i].name = freadnts(file);
        umb->materials[i].texturePath = freadnts(file);
        umb->materials[i].textureBase = freadnts(file);

        FREADS(umb->materials[i].ambient);
        FREADS(umb->materials[i].diffuse);
        FREADS(umb->materials[i].specular);
        FREADS(umb->materials[i].glossiness);
    }

    FREADS(umb->numObjects);
    CALLOC(umb->objects, umb->numObjects, UMBObject);

    for (int i = 0; i < umb->numObjects; i++)
    {
        FREADS(umb->objects[i].materialIndex);
        FREADS(umb->objects[i].numKeyFrames);
        FREADS(umb->objects[i].numAnimationFrames);

        CALLOC(umb->objects[i].frames, umb->objects[i].numKeyFrames, UMBFrame);

        for (int j = 0; j < umb->objects[i].numKeyFrames; j++)
        {
            FREADS(umb->objects[i].frames[j].number);
            FREADS(umb->objects[i].frames[j].usePreviousIndexData);
            FREADS(umb->objects[i].frames[j].usePreviousTextureData);
            FREADS(umb->objects[i].frames[j].numFaces);

            if (umb->objects[i].frames[j].numFaces > 0 && !umb->objects[i].frames[j].usePreviousIndexData)
            {
                FREADP(umb->objects[i].frames[j].indices, sizeof(int16_t), umb->objects[i].frames[j].numFaces * 3);
            }

            FREADS(umb->objects[i].frames[j].numTextures);

            if (umb->objects[i].frames[j].numTextures > 0 && !umb->objects[i].frames[j].usePreviousTextureData)
            {
                FREADP(umb->objects[i].frames[j].textures, sizeof(Vec2), umb->objects[i].frames[j].numTextures);
            }

            FREADS(umb->objects[i].frames[j].numColors);

            if (umb->objects[i].frames[j].numColors > 0 && !umb->objects[i].frames[j].usePreviousTextureData)
            {
                FREADP(umb->objects[i].frames[j].colors, sizeof(OpaqueColor32), umb->objects[i].frames[j].numColors);
            }

            FREADS(umb->objects[i].frames[j].numVertices);

            if (umb->objects[i].frames[j].numVertices > 0)
            {
                UMBVertex* vertex;
                FREADP(vertex, sizeof(UMBVertex), umb->objects[i].frames[j].numVertices);

                umb->objects[i].frames[j].vertices = (UMBVector3*)calloc(umb->objects[i].frames[j].numVertices, sizeof(UMBVector3));
                umb->objects[i].frames[j].normals  = (UMBVector3*)calloc(umb->objects[i].frames[j].numVertices, sizeof(UMBVector3));

                for (int k = 0; k < umb->objects[i].frames[j].numVertices; k++)
                {
                    umb->objects[i].frames[j].vertices[k] = vertex[k].vertex;
                    umb->objects[i].frames[j].normals[k]  = vertex[k].normal;
                }

                free(vertex);
            }
        }
    }

    return umb;
}

void umb_delete(UMB* umb)
{
    if (umb->numMaterials > 0)
    {
        for (int i = 0; i < umb->numMaterials; i++)
        {
            FREE(umb->materials[i].name);
            FREE(umb->materials[i].texturePath);
            FREE(umb->materials[i].textureBase);
        }

        FREE(umb->materials);
    }

    if (umb->numObjects > 0)
    {
        for (int i = 0; i < umb->numObjects; i++)
        {
            if (umb->objects[i].numKeyFrames > 0)
            {
                for (int j = 0; j < umb->objects[i].numKeyFrames; j++)
                {
                    FREE(umb->objects[i].frames[j].indices);
                    FREE(umb->objects[i].frames[j].textures);
                    FREE(umb->objects[i].frames[j].colors);
                    FREE(umb->objects[i].frames[j].vertices);
                    FREE(umb->objects[i].frames[j].normals);
                }

                FREE(umb->objects[i].frames);
            }
        }

        FREE(umb->objects);
    }

    FREE(umb);
}