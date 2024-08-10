
#include <stdio.h>
#include <filestream.h>
#include "umb.h"

int main()
{
    //engine_init();
    Stream* stream = stream_create("dog.umb");

    UMB* umb = umb_from_stream(stream);
    umb_delete(umb);

    stream_close(stream);
    printf("ran successfully!!");

    return 0;
}