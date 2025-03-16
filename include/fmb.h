#pragma once
#include <vector.h>
#include <common.h>
#include <stdint.h>

/*

Foursaken Model Binary

    Known Games

        Bug Heroes                          - SUPPORTED
        Bug Heroes Quest                    - SUPPORTED
        Food Ninja                          - UNSUPPORTED
        Sky Gnomes                          - SUPPORTED
        New York Zombies 2 (very partially) - UNSUPPORTED

    Known Versions

        1.0             - UNSUPPORTED
        1.1             - UNSUPPORTED
        1.2             - UNSUPPORTED
        100 (PUBLISHED) - SUPPORTED

    Header: fmb\0

Status: UNFINISHED

*/

typedef struct FMBMaterial
{
    char* name, *texturePath;

    OpaqueColor ambient, diffuse, specular;

    float glossiness;
} FMBMaterial;

typedef struct FMBFrame
{
    int16_t index, frameNumber, verticesOffset;
} FMBFrame;

typedef struct FMBObject
{
    //most likely unused
    char* name;

    int32_t materialIndex;

    uint16_t hasNormals, hasTextures, hasColors;

    int32_t numKeyFrames;

    FMBFrame* frames;

    int32_t numFaces;

    uint8_t* indices;

    int32_t numVertices;

    uint8_t* vertices, *normals, *textures, *colors;

    Vec3* centers;

    float* radiuses;

    uint16_t* keyFrameLookUp;
} FMBObject;

typedef enum FMBDataType
{
    FMB_BYTE, FMB_UNSIGNED_BYTE, FMB_SHORT, FMB_UNSIGNED_SHORT, FMB_FLOAT
} FMBDataType;

typedef struct FMB
{
    float version;

    FMBDataType indexDataType, vertexDataType, normalDataType, textureDataType, colorDataType;

    int32_t indexDataSize, vertexDataSize, normalDataSize, textureDataSize, colorDataSize;

    float offset, scale;

    int32_t numFrames, numMaterials;

    float inverseScale;

    FMBMaterial* materials;

    int32_t numObjects;

    FMBObject* objects;

    Vec3* mins, *maxes;
} FMB;

typedef struct FMBGLDataHeader
{
    int32_t index, vertex, normal, texture, color;
} FMBGLDataHeader;

FMB* fmb_load(FILE* file);
void fmb_delete(FMB* fmb);