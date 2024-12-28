#pragma once
#include <fmstream.h>
#include <common.h>

#define UMB_EXT 6450549 //int32 for umb\0 (only for internal use, the umb format has no header)
#define FMB_EXT 6450534 //int32 for fmb\0
#define FMB2_EXT 845311334 //int32 for fmb2
#define FMA_EXT 6384998 //int32 for fma\0 (only for internal use, the fma format has a long ascii header)

typedef struct Model
{
    int header;

    float version;

    void* ptr;
} Model;

EXPORT void delete_model(Model* model);

EXPORT Model* load_model(char* path);
