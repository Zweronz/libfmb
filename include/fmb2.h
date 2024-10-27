#pragma once
#include <vector.h>
#include <filestream.h>
#include <common.h>
#include <stdbool.h>

typedef enum
{
    UVChannel0, UVChannel1, UVChannel2, UVChannel3, 
    Position, Normal, Tangent, Binormal, Color
} FMB2VertexChannelType;

typedef struct
{
    FMB2VertexChannelType exportedType;

    int dataType, dataSize, components, offsets;
} FMB2VertexChannel;

typedef struct
{
    char* name;

    int materialIndex, numFaces, numVertices, numKeyFrames, drawDataSize, numVertexChannels;

    FMB2VertexChannel* vertexChannels;
} FMB2Object;

typedef struct
{
    float offset, scale;

    int numMaterials, numObjects, numFrames, numKeyFrames;

    FMB2Object* objects;
} FMB2Data;

typedef struct
{
    unsigned short* keyFrameLookUp;
} FMB2Anim;

typedef struct
{
    int numFrames;

    unsigned short* frameLookUp;

    float* radiuses;

    Vec3* mins, *maxes, *centers;

    
} FMB2ObjectBounds;

typedef struct
{
    FMB2ObjectBounds* bounds;
} FMB2Bnds;

typedef struct
{
    char* name;

    Vec3* positions, *rotations;
} FMB2Dummy;

typedef struct
{
    int numDummies;

    FMB2Dummy* dummies;
} FMB2Dums;

typedef struct
{
    unsigned short* indices;
} FMB2ObjectFaces;

typedef struct 
{
    FMB2ObjectFaces* faces;
} FMB2Face;

typedef struct
{
    char* vertexData;

    unsigned short* frameLookUp;
} FMB2ObjectChannel;

typedef struct
{
    FMB2ObjectChannel* channels;
} FMB2VertexData;

typedef struct
{
    FMB2VertexData* data;
} FMB2Chnd;

typedef struct
{
    void* idk;
} FMB2Bone;

typedef struct
{
    int label;

    size_t length;
} FMB2Chunk;

typedef struct
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

typedef enum
{
    DATA = 1635017060, //int32 of bytes for 'data'
    ANIM = 1835626081, //int32 of bytes for 'anim'
    BNDS = 1935961698, //int32 of bytes for 'bnds'
    DUMS = 1936553316, //int32 of bytes for 'dums'
    FACE = 1701011814, //int32 of bytes for 'face'
    CHND = 1684957283, //int32 of bytes for 'chnd'
    BONE = 1701736290, //int32 of bytes for 'bone'
} FMB2ChunkType;

FMB2* fmb2_from_stream(Stream* stream);

void fmb2_delete(FMB2* fmb2);

char* ptr_from_fmb2(FMB2* fmb2);

void fmb2_read_data(Stream* stream, FMB2* fmb2, size_t length);

void fmb2_read_anim(Stream* stream, FMB2* fmb2, size_t length);

void fmb2_read_bnds(Stream* stream, FMB2* fmb2, size_t length);

void fmb2_read_dums(Stream* stream, FMB2* fmb2, size_t length);

void fmb2_read_face(Stream* stream, FMB2* fmb2, size_t length);

void fmb2_read_chnd(Stream* stream, FMB2* fmb2, size_t length);

void fmb2_read_bone(Stream* stream, FMB2* fmb2, size_t length);