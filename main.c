
#include <stdio.h>
#include <stdlib.h>
#include <loader.h>
#include <umb.h>
#include <common.h>

#if _WIN32
#include <profileapi.h>
#endif

int main(int argc, char** argv)
{
    int runCount = 1;

#if _WIN32
    LARGE_INTEGER buf;

    QueryPerformanceCounter(&buf);
    size_t timeStart = buf.QuadPart;

    QueryPerformanceFrequency(&buf);
    size_t frequency = buf.QuadPart;
#endif

    if (argc > 2)
    {
        runCount = atoi(argv[2]);
    }

    for (int i = 0; i < runCount; i++)
    {
        Model* model = load_model(argv[1]);
        delete_model(model);
    }

#if _WIN32
    QueryPerformanceCounter(&buf);
    size_t timeEnd = buf.QuadPart;

    printf("time elapsed: %f\n", (timeEnd - timeStart * 1.0) / frequency);
    printf("time avg: %f\n", ((timeEnd - timeStart * 1.0) / frequency) / runCount);
#endif

    DEBUG("ran successfully");

    return 0;
}