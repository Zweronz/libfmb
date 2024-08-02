#pragma once
#define byte unsigned char

typedef struct
{
    float x, y, z;
} Vector3;

typedef struct
{
    float x, y;
} Vector2;

typedef struct
{
    short x, y, z;
} HalfVector3;

typedef struct
{
    short x, y, z;
} HalfVector2;

typedef struct
{
    float r, g, b;
} Color;

typedef struct
{
    byte r, g, b;
} Color32;