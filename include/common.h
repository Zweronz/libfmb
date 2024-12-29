#pragma once
#include <string.h>
#include <malloc.h>
#include <stdio.h>

#define FOREACH(n, l) for (int n = 0; n < l; n++)
#define CALLOC(t, l) (t*)calloc(l, sizeof(t))
#define FREE(p) if (p != NULL) free(p)

#ifdef _WIN32
    #define EXPORT __declspec(dllexport)
#else
    #define EXPORT __attribute__((visibility("default")))
#endif

void debug(const char* str);