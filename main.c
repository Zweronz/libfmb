
#include <stdio.h>
#include <filestream.h>
#include <fmb2.h>

int main()
{
    Stream* stream = stream_create("hero_aphid.fmb2");

    FMB2* fmb2 = fmb2_from_stream(stream);
    fmb2_delete(fmb2);

    stream_close(stream);
    printf("ran successfully!!");

    return 0;
}