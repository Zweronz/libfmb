#pragma once
#include <vector.h>
#include <stdint.h>

/*

Unison Model Binary

    Known Games

        New York Zombies - SUPPORTED

    Known Versions: N\A
    
    Header: N\A

Status: DONE

*/

typedef struct UMBVector3
{
    float x, z, y;
} UMBVector3;

typedef struct UMBVertex
{
    UMBVector3 vertex, normal;
} UMBVertex; //vertex/normals are interleaved

typedef struct UMBFrame
{
    int32_t number;

    uint16_t usePreviousIndexData, usePreviousTextureData;

    int32_t numFaces;

    uint16_t* indices;

    int32_t numTextures;

    Vec2* textures;

    int32_t numColors;

    OpaqueColor32* colors;

    int32_t numVertices;

    UMBVector3* vertices, *normals;
} UMBFrame;

typedef struct UMBObject
{
    int32_t materialIndex, numKeyFrames, numAnimationFrames;

    UMBFrame* frames;
} UMBObject;

typedef struct UMBMaterial
{
    char* name, *texturePath, *textureBase;

    OpaqueColor ambient, diffuse, specular;

    float glossiness;
} UMBMaterial;

typedef struct UMB
{
    int32_t numMaterials;

    UMBMaterial* materials;

    int32_t numObjects;

    UMBObject* objects;
} UMB;

UMB* umb_load(FILE* file);
void umb_delete(UMB* umb);