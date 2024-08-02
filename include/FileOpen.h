#pragma once
#include <stdio.h>
#include <FileStream.h>
#include <malloc.h>

typedef struct
{
    char* buf;

    size_t size;
} File;

File* open_file(const char* path)
{
    FILE* file = fopen(path, "rb");

    if (!file)
    {
        printf("error opening file");
        return NULL;
    }

    File* openFile;
    openFile = (File*)malloc(sizeof(File));
    
    fseek(file, 0, SEEK_END);
    openFile->size = ftell(file);
    fseek(file, 0, SEEK_SET);

    openFile->buf = (char*)malloc(openFile->size + 1);
    fread(openFile->buf, sizeof(char), openFile->size, file);

    return openFile;
}

void file_close(File* file)
{
    if (file != NULL)
    {
        if (file->buf != NULL)
        {
            free(file->buf);
        }

        free(file);
    }
}