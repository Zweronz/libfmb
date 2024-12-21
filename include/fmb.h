#pragma once
#include <vector.h>
#include <fmstream.h>
#include <malloc.h>
#include <common.h>

//Foursaken Model Binary

//Known games

//Bug Heroes
//Bug Heroes Quest

//Known versions

//1.0
//1.1
//1.2

//Header: fmb\0

typedef struct FMBMaterial
{
    char* name, *texturePath;

    OpaqueColor ambient, diffuse, specular;

    float glossiness;
} FMBMaterial;

typedef struct FMBFrame
{
    short index, frameNumber, verticesOffset;
} FMBFrame;

typedef struct FMBObject
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

typedef enum FMBDataType
{
    FMB_BYTE, FMB_UNSIGNED_BYTE, FMB_SHORT, FMB_UNSIGNED_SHORT, FMB_FLOAT
} FMBDataType;

typedef struct FMB
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

EXPORT FMB* fmb_from_stream(Stream* stream);

EXPORT void fmb_delete(FMB* fmb);

char* ptr_from_fmb(FMB* fmb);