#pragma once
#include <filestream.h>

#define UMB_EXT 6450549 //int32 for umb\0 (only for internal use, the umb format has no header)
#define FMB_EXT 6450534 //int32 for fmb\0
#define FMB2_EXT 845311334 //int32 for fmb2
#define FMA_EXT 6384998 //int32 for fma\0 (only for internal use, the fma format has a long ascii header)

typedef struct
{
    int header;

    void* ptr;
} Model;

void delete_model(Model* model);

Model* load_model(const char* path);
