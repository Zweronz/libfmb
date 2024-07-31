#pragma once
#include <stdio.h>
#include <stdint.h>

//why
typedef struct
{
    void* ptr, *file;
    
    size_t size;
} Stream;

Stream* stream_open(void* ptr, size_t size);

void stream_close(Stream* stream);

void stream_advance (Stream* stream, size_t amt);

char* stream_string(Stream* stream);

uint8_t stream_byte(Stream* stream);

short stream_short(Stream* stream);

int stream_int(Stream* stream);

float stream_float(Stream* stream);

void* stream_data(Stream* stream);