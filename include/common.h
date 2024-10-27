#pragma once
#include <string.h>
#include <malloc.h>

#define FOREACH(n, l) for (int n = 0; n < l; n++)
#define CALLOC(t, l) (t*)calloc(l, sizeof(t))
#define FREE(p) if (p != NULL) free(p)

#define DEBUG

inline void debug(const char* str)
{
#ifdef DEBUG
    printf(str);
#endif
}