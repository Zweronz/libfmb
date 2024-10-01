#pragma once
#include <vector.h>
#include <filestream.h>
#include <malloc.h>

#define STREAM_VAL(v, t) fmb->v = stream_##t(stream)

typedef struct
{
    char* name, *texturePath;

    OpaqueColor ambient, diffuse, specular;

    float glossiness;
} FMBMaterial;

typedef struct
{
    short index, frameNumber, verticesOffset;
} FMBFrame;

typedef struct
{
    //most likely unused
    char* name;

    int materialIndex;

    bool hasNormals, hasTextures, hasColors;

    int numKeyFrames;

    FMBFrame* frames;

    int numFaces, numVertices;

    char* indices, *vertices, *normals, *textures, *colors;

    Vec3* centers;

    float* radiuses;

    short* keyFrameLookUp;
} FMBObject;

typedef enum
{
    BYTE, UNSIGNED_BYTE, SHORT, UNSIGNED_SHORT, FLOAT
} FMBDataType;

typedef struct
{
    float version;

    int indexDataSize, vertexDataSize, normalDataSize, textureDataSize, colorDataSize;

    float offset, scale, inverseScale;

    int numFrames, numMaterials;

    FMBMaterial* materials;

    int numObjects;

    FMBObject* objects;

    Vec3* mins, *maxes;
} FMB;

FMBDataType numToDataType(int num);

int dataSize(FMBDataType dataType);

FMB* fmb_from_stream(Stream* stream);

void fmb_delete(FMB* fmb);

char* ptr_from_fmb(FMB* fmb);