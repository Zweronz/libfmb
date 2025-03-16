#pragma once
#include <vector.h>
#include <common.h>
#include <stdint.h>

/*

Foursaken Model Binary V2

    Known Games

        New York Zombies 2
        Heroes and Castles
        Block Fortress
        Monster Adventures
        Bug Heroes 2
        Block Fortress: War
        Phantom Rift
        Puzzle to the Center of Earth
        Heroes and Castles 2
        All is Lost
        War Tortoise
        Adventure Company
        Color Bots
        Noblemen: 1896
        Game of Gods
        Block Fortress: Empires
        War Tortoise 2
        Magic vs Metal
        Bug Heroes: Tower Defense

    Known Versions

        1.0  - SUPPORTED?
        1.01 - SUPPORTED
        1.02 - SUPPORTED
        1.03 - SUPPORTED
        1.1  - SUPPORTED

    Header: fmb2\0

Status: UNTESTED

*/

typedef enum FMB2VertexChannelType
{
    UVChannel0, UVChannel1, UVChannel2, UVChannel3, 
    Position, Normal, Tangent, Binormal, Color
} FMB2VertexChannelType;

typedef struct FMB2Chunk
{
    int32_t label;

    int32_t length;
} FMB2Chunk;

typedef struct FMB2VertexChannel
{
    FMB2VertexChannelType exportedType;

    int32_t dataType, dataSize, numComponents, numOffsets;

    uint8_t* data;

    uint16_t* keyFrameToOffset;
} FMB2VertexChannel;

typedef struct FMB2Model
{
    char* name;

    int32_t materialIndex, numFaces, numVertices, indexDataType, indexDataSize, numKeyFrames;

    uint8_t* indices;

    int32_t numChannels;

    FMB2VertexChannel* channels;

    int32_t numBoundingOffsets;

    Vec4* boundingSpheres;

    Vec3* mins, *maxes;

    uint16_t* boundingOffsetToKeyFrame;
} FMB2Model;

typedef struct FMB2Dummy
{
    char* name;
    
    uint8_t* frameData;
} FMB2Dummy;

typedef struct FMB2
{
    int32_t numChunks;

    float version, offset, scale;

    int32_t numKeyFrames, numFrames;

    uint16_t* frameToKeyFrame, *keyFrameToFrameNumber;

    int32_t numMaterials, numModels, numDummies;

    FMB2Model* models;

    FMB2Dummy* dummies;
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

FMB2* fmb2_load(FILE* file);
void fmb2_delete(FMB2* fmb2);