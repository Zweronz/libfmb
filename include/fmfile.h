#pragma once
#include <stdio.h>
#include <malloc.h>

typedef struct File
{
    char* buf;

    size_t size;
} File;

File* file_open(char* path);
void file_close(File* file);