#pragma once
#include <fmstream.h>
#include <vector.h>
#include <stdbool.h>

/*

Unison Model Binary

    Known Games

        New York Zombies - SUPPORTED

    Known Versions: N\A
    
    Header: N\A

Status: DONE

*/

#define UMB_FRAME_SIZE 24
#define UMB_OBJECT_SIZE 12
#define UMB_MATERIAL_SIZE 42
#define UMB_SIZE 8

typedef struct UMBVector3
{
    float x, z, y;
} UMBVector3;

typedef struct UMBFrame
{
    int number;

    bool usePreviousIndexData, usePreviousTextureData;

    int numFaces;

    unsigned short* indices;

    int numTextures;

    Vec2* textures;

    int numColors;

    OpaqueColor32* colors;

    int numVertices;

    UMBVector3* vertices, *normals;
} UMBFrame;

typedef struct UMBObject
{
    int materialIndex, numKeyFrames, numAnimationFrames;

    UMBFrame* frames;
} UMBObject;

typedef struct UMBMaterial
{
    char* name, *texturePath, *textureBase;

    bool hasTexture;

    OpaqueColor ambient, diffuse, specular;

    float glossiness;
} UMBMaterial;

typedef struct UMB
{
    int numMaterials;

    UMBMaterial* materials;

    int numObjects;

    UMBObject* objects;
} UMB;

UMB* umb_from_stream(Stream* stream);

void umb_delete(UMB* umb);

char* ptr_from_umb(UMB* umb);

size_t umb_calc_size(UMB* umb);