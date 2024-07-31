#include "Stream.h"
#include <string.h>
#include <malloc.h>

Stream* stream_open(void* ptr, size_t size)
{
    if (ptr == NULL)
    {
        return NULL;
    }

    Stream* stream;
    stream = (Stream*)malloc(sizeof(Stream));

    if (stream == NULL)
    {
        return NULL;
    }

    stream->size = size;

    stream->ptr = ptr;
    stream->file = malloc(size);

    if (stream-> file == NULL)
    {
        free(stream);
        return NULL;
    }

    memcpy(stream->file, ptr, size);

    return stream;
}

void stream_close(Stream* stream)
{
    if (stream != NULL)
    {
        if (stream->ptr != NULL)
        {
            free(stream->ptr);
        }

        if (stream->file != NULL)
        {
            free(stream->file);
        }

        free(stream);
    }
}

void stream_advance(Stream* stream, size_t amt)
{
    if (stream == NULL || stream->ptr == NULL || stream->file == NULL)
    {
        printf("invalid stream!");
        return;
    }

    stream->file = (char*)stream->file + amt;
}