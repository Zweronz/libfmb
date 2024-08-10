#pragma once
#define byte unsigned char

typedef struct
{
    float x, y, z;
} Vec3;

typedef struct
{
    float x, y;
} Vec2;

typedef struct
{
    short x, y, z;
} HalfVec3;

typedef struct
{
    short x, y, z;
} HalfVec2;

typedef struct
{
    float r, g, b;
} OpaqueColor;

typedef struct
{
    byte r, g, b;
} OpaqueColor32;