#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

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