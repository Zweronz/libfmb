#include <filestream.h>
#include <fileopen.h>
#include <string.h>
#include <malloc.h>
#include <common.h>

Stream* stream_create(const char* path)
{
    File* file = file_open(path);
    Stream* stream = stream_open(file->buf, file->size);

    file_close(file);
    return stream;
}

Stream* stream_open(char* ptr, size_t size)
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
    stream->ptr = (char*)malloc(size + 1);

    stream->pos = 0;

    memcpy(stream->ptr, ptr, size);

    return stream;
}

void stream_close(Stream* stream)
{
    if (stream != NULL)
    {
        FREE(stream->ptr);

        free(stream);
    }
}

void stream_advance(Stream* stream, size_t amt)
{
    if (stream == NULL || stream->ptr == NULL)
    {
        printf("invalid stream!\n");
        return;
    }

    if (stream->pos + amt > stream->size)
    {
        printf("attempted to read past size!\n");
        return;
    }

    stream->pos += amt;
}

char* stream_string(Stream* stream)
{
    int len = 0;

    for (int i = 0;; i++)
    {
        len++;

        if (*(stream->ptr + stream->pos + i) == '\0')
        {
            break;
        }
    }

    char* str = (char*)malloc(len + 1);
    memcpy(str, stream->ptr + stream->pos, len);

    str[len] = '\0';

    stream_advance(stream, len);
    return str;
}

short stream_short(Stream* stream)
{
    void* ptr = stream_data(stream, sizeof(short));
    short num = *(short*)ptr;

    free(ptr);

    return num;
}

int stream_int(Stream* stream)
{
    void* ptr = stream_data(stream, sizeof(int));
    int num = *(int*)ptr;

    free(ptr);

    return num;
}

float stream_float(Stream* stream)
{
    void* ptr = stream_data(stream, sizeof(float));
    float num = *(float*)ptr;

    free(ptr);

    return num;
}

void* stream_data(Stream* stream, size_t len)
{
    void* buf = malloc(len);
    memcpy(buf, stream->ptr + stream->pos, len);

    stream_advance(stream, len);
    return buf;
}

bool stream_short_bool(Stream* stream)
{
    return stream_short(stream) == 1;
}

void stream_debug(Stream* stream)
{
    printf("pos: %zu size: %zu\n", stream->pos, stream->size);
}