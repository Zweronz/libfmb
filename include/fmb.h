#pragma once
#include <vector.h>
#include <fmstream.h>
#include <malloc.h>
#include <common.h>

/*

    Foursaken Model Binary

        Known Games

            Bug Heroes       - SUPPORTED
            Bug Heroes Quest - SUPPORTED
            Food Ninja       - UNSUPPORTED (causes memory leak?)
            Sky Gnomes       - SUPPORTED

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

    FMBDataType indexDataType, vertexDataType, normalDataType, textureDataType, colorDataType;

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