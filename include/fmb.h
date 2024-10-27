#pragma once
#include <vector.h>
#include <filestream.h>
#include <malloc.h>

//Foursaken Model Binary

//Known games

//Bug Heroes
//Bug Heroes Quest

//Known versions

//1.0
//1.1
//1.2

//Header: fmb\0

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
    FMB_BYTE, FMB_UNSIGNED_BYTE, FMB_SHORT, FMB_UNSIGNED_SHORT, FMB_FLOAT
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

FMB* fmb_from_stream(Stream* stream);

void fmb_delete(FMB* fmb);

char* ptr_from_fmb(FMB* fmb);