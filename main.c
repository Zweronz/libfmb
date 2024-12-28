
#include <stdio.h>
#include <loader.h>
#include <umb.h>
#include <common.h>
#include <fmstream.h>

int main(int argc, char** argv)
{
    Model* model = load_model(argv[1]);
    delete_model(model);

    debug("ran successfully");

    return 0;
}