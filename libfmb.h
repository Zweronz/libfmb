#pragma once
#include <stdint.h>

typedef struct Vec4
{
    float x, y, z, w;
} Vec4;

typedef struct Vec3
{
    float x, y, z;
} Vec3;

typedef struct Vec2
{
    float x, y;
} Vec2;

typedef struct HalfVec3
{
    short x, y, z;
} HalfVec3;

typedef struct HalfVec2
{
    short x, y, z;
} HalfVec2;

typedef struct OpaqueColor
{
    float r, g, b;
} OpaqueColor;

typedef struct OpaqueColor32
{
    unsigned char r, g, b;
} OpaqueColor32;

typedef struct UMBVector3
{
    float x, z, y;
} UMBVector3;

typedef struct UMBVertex
{
    UMBVector3 vertex, normal;
} UMBVertex; //vertex/normals are interleaved

typedef struct UMBFrame
{
    int32_t number;

    uint16_t usePreviousIndexData, usePreviousTextureData;

    int32_t numFaces;

    uint16_t* indices;

    int32_t numTextures;

    Vec2* textures;

    int32_t numColors;

    OpaqueColor32* colors;

    int32_t numVertices;

    UMBVector3* vertices, *normals;
} UMBFrame;

typedef struct UMBObject
{
    int32_t materialIndex, numKeyFrames, numAnimationFrames;

    UMBFrame* frames;
} UMBObject;

typedef struct UMBMaterial
{
    char* name, *texturePath, *textureBase;

    OpaqueColor ambient, diffuse, specular;

    float glossiness;
} UMBMaterial;

typedef struct UMB
{
    int32_t numMaterials;

    UMBMaterial* materials;

    int32_t numObjects;

    UMBObject* objects;
} UMB;

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

typedef struct Model
{
    int header;

    float version;

    void* ptr;
} Model;

void delete_model(Model* model);
Model* load_model(char* path);