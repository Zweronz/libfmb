
#include <stdio.h>
#include <filestream.h>
#include <fmb.h>

int main()
{
    Stream* stream = stream_create("main_menu.fmb");

    FMB* fmb = fmb_from_stream(stream);
    fmb_delete(fmb);

    stream_close(stream);
    printf("ran successfully!!");

    return 0;
}