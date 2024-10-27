
#include <stdio.h>
#include <loader.h>
#include <common.h>

int main(int argc, char** argv)
{
    Model* model = load_model("enemy_ladybug.fmb2");
    delete_model(model);

    debug("ran successfully");

    return 0;
}