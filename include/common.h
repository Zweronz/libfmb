#pragma once
#include <string.h>
#include <malloc.h>
#include <stdio.h>
#include <stddef.h>

#define FREE(p) if (p != NULL) free(p)
#define CALLOC(v, c, t) v = (t*)calloc(c, sizeof(t))

//#define DEBUG_BUILD

//assumes a file pointer named "file" is in the scope
#define FREAD(b, s) fread(b, s, 1, file) //read s bytes into b
#define FREADP(b, s, c) b = calloc(c, s); fread(b, s, c, file) //read s * c bytes into b
#define FREADA(b, s) b = malloc(s); fread(b, s, 1, file) //read s bytes into b
#define FREADS(v) fread(&v, sizeof(v), 1, file) //read into v

#ifdef _WIN32
    #define EXPORT __declspec(dllexport)
#else
    #define EXPORT __attribute__((visibility("default")))
#endif

#ifdef DEBUG_BUILD
    #define DEBUG(s) printf(s)
    #define DEBUGF(s, p) printf(s, p)
#else
    #define DEBUG(s)
    #define DEBUGF(s, p)
#endif

char* freadnts(FILE* file);
void fadvance(size_t adv, FILE* file);
void fprintpos(FILE* file);