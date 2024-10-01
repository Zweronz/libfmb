#pragma once
#include <string.h>

#define FOR(n) for (int i = 0; i < n; i++)
#define FOR_N(i, n) for (int i = 0; i < n; i++)

#define FOR_R(l, h) for (int i = l; i < h; i++)
#define FOR_N_R(i, l, h) for (int i = l; i < h; i++)

#define FOR_F(n, f) for (int i = 0; i < n; f)
#define FOR_N_F(i, n, f) for (int i = 0; i < n; f)

#define FOR_R_F(l, h, f) for (int i = l; i < h; f)
#define FOR_N_R_F(i, l, h, f) for (int i = l; i < h; f)

#define FREE(p) if (p != NULL) free(p)