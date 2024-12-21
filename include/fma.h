#pragma once
#include <vector.h>

typedef enum FMADataType
{
    FMA_BYTE = 2, FMA_UNSIGNED_BYTE = 3, FMA_SHORT = 4, FMA_UNSIGNED_SHORT = 5, FMA_FIXED = 6, FMA_FLOAT = 7
} FMADataType;

typedef struct FMAMaterial
{
    char* name, texture;

    OpaqueColor32 ambient, diffuse, specular;

    float glossiness;
} FMAMaterial;

typedef struct FMA
{
    float version;

    int numMaterials, colorMaterialIndex;

    FMAMaterial* materials;
} FMA;