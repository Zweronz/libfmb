#include <loader.h>

#include <umb.h>
#include <fmb.h>
#include <fmb2.h>

void delete_model(Model* model)
{
    if (model != NULL)
    {
        switch (model->header)
        {
            case FMB_EXT:
                fmb_delete((FMB*)model->ptr);
                break;

            case FMB2_EXT:
                fmb2_delete((FMB2*)model->ptr);
                break;

            case UMB_EXT:
                umb_delete((UMB*)model->ptr);
                break;
        }

        free(model);
    }
}

Model* load_model(char* path)
{
    DEBUG("loading model\n");

    FILE* file = fopen(path, "rb");
    
    Model* model = (Model*)malloc(sizeof(Model));
    FREADS(model->header);

    switch (model->header)
    {
        case FMB_EXT:
            DEBUG("model is type FMB\n");

            model->ptr = fmb_load(file);
            model->version = ((FMB*)model->ptr)->version;

            break;

        case FMB2_EXT:
            DEBUG("model is type FMB2\n");

            //skip null terminator on header
            fadvance(1, file);

            model->ptr = fmb2_load(file);
            model->version = ((FMB2*)model->ptr)->version;

            break;

        default:
            DEBUG("model is type UMB\n");

            fpos_t pos = 0;
            fsetpos(file, &pos);

            model->ptr = umb_load(file); //umb has no header!
            model->header = UMB_EXT;

            break;
    }

    fclose(file);

    return model;
}