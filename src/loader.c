#include <loader.h>

#include <umb.h>
#include <fmb.h>
#include <fmb2.h>

void delete_model(Model* model)
{
    if (model != NULL)
    {
        #define IMPL_MODEL(mod, ext) case ext##_EXT: mod##_delete((ext*)model->ptr); break

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

        #undef IMPL_MODEL

        free(model);
    }
}

Model* load_model(char* path)
{
    debug("loading model\n");

    Stream* stream = stream_create(path);
    
    Model* model = (Model*)malloc(sizeof(Model));
    model->header = stream_int(stream);

    printf("%zu\n", stream->size);

    switch (model->header)
    {
        case FMB_EXT:
            debug("model is type FMB\n");

            model->ptr = fmb_from_stream(stream);
            model->version = ((FMB*)model->ptr)->version;

            break;

        case FMB2_EXT:
            debug("model is type FMB2\n");

            //skip null terminator on header
            ADVANCE(1);

            model->ptr = fmb2_from_stream(stream);

            break;

        default:
            debug("model is type UMB\n");

            stream->pos = 0;
            model->ptr = umb_from_stream(stream); //umb has no header!
            model->header = UMB_EXT;

            break;
    }

    stream_close(stream);

    return model;
}