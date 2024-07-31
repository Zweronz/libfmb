#include <stdio.h>
#include <Stream.h>
#include <malloc.h>

typedef struct
{
    void* buf;

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

    openFile->buf = malloc(openFile->size + 1);
    fread(openFile->buf, openFile->size, 1, file);

    return openFile;
}