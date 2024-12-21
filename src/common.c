#include <common.h>

void debug(const char* str)
{
#ifdef DEBUG
    printf(str);
#endif
}