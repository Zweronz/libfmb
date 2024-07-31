
#include <stdio.h>
#include <Stream.h>
#include <FileOpen.h>

int main()
{
    File* file = open_file("test");
    Stream* stream = stream_open(file->buf, file->size);

    printf("%d", file->size);

    stream_advance(stream, 8);
    stream_close(stream);

    free(file);

    return 0;
}