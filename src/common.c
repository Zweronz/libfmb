#include <common.h>

char* freadnts(FILE* file)
{
    fpos_t pos;
    fgetpos(file, &pos);

    size_t length = 0;

    while (1)
    {
        length++;

        if (fgetc(file) == 0)
        {
            break;
        }
    }

    fsetpos(file, &pos);

    char* str = (char*)malloc(length);
    fread(str, 1, length, file);

    return str;
}

void fadvance(size_t adv, FILE* file)
{
    fpos_t pos;
    
    fgetpos(file, &pos);
    pos += adv;

    fsetpos(file, &pos);
}

void fprintpos(FILE* file)
{
    fpos_t pos = 0;
    fgetpos(file, &pos);

    printf("%zu\n", pos);
}