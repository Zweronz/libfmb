#pragma once
#include <vector.h>
#include <fmstream.h>
#include <common.h>
#include <stdbool.h>

typedef enum FMB2VertexChannelType
{
    UVChannel0, UVChannel1, UVChannel2, UVChannel3, 
    Position, Normal, Tangent, Binormal, Color
} FMB2VertexChannelType;

typedef struct FMB2VertexChannel
{
    FMB2VertexChannelType exportedType;

    int dataType, dataSize, components, offsets;
} FMB2VertexChannel;

typedef struct FMB2Object
{
    char* name;

    int materialIndex, numFaces, numVertices, numKeyFrames, drawDataSize, numVertexChannels;

    FMB2VertexChannel* vertexChannels;
} FMB2Object;

typedef struct FMB2Data
{
    float offset, scale;

    int numMaterials, numObjects, numFrames, numKeyFrames;

    FMB2Object* objects;
} FMB2Data;

typedef struct FMB2Anim
{
    unsigned short* keyFrameLookUp;
} FMB2Anim;

typedef struct FMB2ObjectBounds
{
    int numFrames;

    unsigned short* frameLookUp;

    float* radiuses;

    Vec3* mins, *maxes, *centers;
} FMB2ObjectBounds;

typedef struct FMB2Bnds
{
    FMB2ObjectBounds* bounds;
} FMB2Bnds;

typedef struct FMB2Dummy
{
    char* name;

    Vec3* positions, *rotations;
} FMB2Dummy;

typedef struct FMB2Dums
{
    int numDummies;

    FMB2Dummy* dummies;
} FMB2Dums;

typedef struct FMB2ObjectFaces
{
    unsigned short* indices;
} FMB2ObjectFaces;

typedef struct FMB2Face
{
    FMB2ObjectFaces* faces;
} FMB2Face;

typedef struct FMB2ObjectChannel
{
    char* vertexData;

    unsigned short* frameLookUp;
} FMB2ObjectChannel;

typedef struct FMB2VertexData
{
    FMB2ObjectChannel* channels;
} FMB2VertexData;

typedef struct FMB2Chnd
{
    FMB2VertexData* data;
} FMB2Chnd;

typedef struct FMB2Bone
{
    void* idk;
} FMB2Bone;

typedef struct FMB2Chunk
{
    int label;

    size_t length;
} FMB2Chunk;

typedef struct FMB2
{
    float version;

    int chunkCount;

    FMB2Data data;

    FMB2Anim anim;

    FMB2Bnds bnds;

    FMB2Dums dums;

    FMB2Face face;

    FMB2Chnd chnd;

    FMB2Bone bone;
} FMB2;

typedef enum FMB2ChunkType
{
    DATA = 1635017060, //int32 of bytes for 'data'
    ANIM = 1835626081, //int32 of bytes for 'anim'
    BNDS = 1935961698, //int32 of bytes for 'bnds'
    DUMS = 1936553316, //int32 of bytes for 'dums'
    FACE = 1701011814, //int32 of bytes for 'face'
    CHND = 1684957283, //int32 of bytes for 'chnd'
    BONE = 1701736290, //int32 of bytes for 'bone'
} FMB2ChunkType;

EXPORT FMB2* fmb2_from_stream(Stream* stream);

EXPORT void fmb2_delete(FMB2* fmb2);

char* ptr_from_fmb2(FMB2* fmb2);