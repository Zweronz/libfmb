#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <fmfile.h>
#include <common.h>

#define STREAM_ARR(t, l) (t*)stream_data(stream, sizeof(t) * l)
#define STREAM_DATA(t) (t*)stream_data(stream, sizeof(t))
#define ADVANCE(n) stream_advance(stream, n)

#define STREAM(t) stream_##t(stream)

typedef struct Stream
{
    char* ptr;
    
    size_t pos, size;
} Stream;

Stream* stream_open(char* ptr, size_t size);
Stream* stream_create(char* path);

void stream_close(Stream* stream);
void stream_advance(Stream* stream, size_t amt);

void* stream_data(Stream* stream, size_t len);
char* stream_string(Stream* stream);
short stream_short(Stream* stream);
int stream_int(Stream* stream);
float stream_float(Stream* stream);
bool stream_short_bool(Stream* stream);
void stream_debug(Stream* stream);

void stream_write(char* buf, size_t* pos, void* data, size_t size);