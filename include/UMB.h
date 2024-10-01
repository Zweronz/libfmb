#pragma once
#include <filestream.h>
#include <vector.h>
#include <stdbool.h>

#define STREAM_VAL(v, t) umb->v = stream_##t(stream)

typedef struct
{
    float x, z, y;
} UMBVector3;

typedef struct
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

typedef struct
{
    int materialIndex, numKeyFrames, numAnimationFrames;

    UMBFrame* frames;
} UMBObject;

typedef struct
{
    char* name, *texturePath, *textureBase;

    bool hasTexture;

    OpaqueColor ambient, diffuse, specular;

    float glossiness;
} UMBMaterial;

typedef struct
{
    int numMaterials;

    UMBMaterial* materials;

    int numObjects;

    UMBObject* objects;
} UMB;

UMB* umb_from_stream(Stream* stream);

void umb_delete(UMB* umb);

char* ptr_from_umb(UMB* umb);