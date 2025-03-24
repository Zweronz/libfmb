#pragma once

#include <common.h>
#define LERP(x, y, t) (x + (y - x) * t)

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