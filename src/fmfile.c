#include <fmfile.h>
#include <common.h>

File* file_open(char* path)
{
    FILE* file = fopen(path, "rb");

    if (!file)
    {
        debug("error opening file\n");
        return NULL;
    }

    File* openFile;
    openFile = (File*)malloc(sizeof(File));
    
    fseek(file, 0, SEEK_END);
    openFile->size = ftell(file);
    fseek(file, 0, SEEK_SET);

    openFile->buf = (char*)malloc(openFile->size + 1);
    fread(openFile->buf, sizeof(char), openFile->size, file);

    fclose(file);

    return openFile;
}

void file_close(File* file)
{
    if (file != NULL)
    {
        if (file->buf != NULL)
        {
            free(file->buf);
        }

        free(file);
    }
}