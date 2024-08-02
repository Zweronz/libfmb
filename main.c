
#include <stdio.h>
#include <FileStream.h>
#include <FileOpen.h>
#include <UMB.h>

int main()
{
    File* file = open_file("test");
    Stream* stream = stream_open(file->buf, file->size);

    file_close(file);

    UMB* umb = umb_from_stream(stream);
    umb_delete(umb);

    stream_close(stream);
    printf("ran successfully!!");

    return 0;
}