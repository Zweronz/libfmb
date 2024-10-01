#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define STREAM_ARR(t, l) (t*)stream_data(stream, sizeof(t) * l)
#define STREAM_DATA(t) (t*)stream_data(stream, sizeof(t))
#define ALLOC_ARR(t, l) (t*)malloc(sizeof(t) * l)
#define ALLOC_DATA(t) (t*)malloc(sizeof(t))
#define ADVANCE(n) stream_advance(stream, n)

typedef struct
{
    char* ptr;
    
    size_t pos, size;
} Stream;

Stream* stream_create(const char* path);

Stream* stream_open(char* ptr, size_t size);

void stream_close(Stream* stream);

void stream_advance (Stream* stream, size_t amt);

char* stream_string(Stream* stream);

short stream_short(Stream* stream);

int stream_int(Stream* stream);

float stream_float(Stream* stream);

void* stream_data(Stream* stream, size_t size);

bool stream_short_bool(Stream* stream);

void stream_debug(Stream* stream);