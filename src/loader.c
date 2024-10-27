#include <loader.h>

#include <umb.h>
#include <fmb.h>
#include <fmb2.h>

void delete_model(Model* model)
{
    if (model != NULL)
    {
        #define IMPL_MODEL(mod, ext) case ext##_EXT: mod##_delete((ext*)model->ptr)

        switch (model->header)
        {
            IMPL_MODEL(fmb,   FMB);
            IMPL_MODEL(fmb2, FMB2);

            default: umb_delete((UMB*)model->ptr); //umb has no header!
        }

        #undef IMPL_MODEL

        free(model);
    }
}

Model* load_model(const char* path)
{
    debug("loading model");

    Stream* stream = stream_create(path);
    
    Model* model = (Model*)malloc(sizeof(Model));
    model->header = stream_int(stream);

    #define IMPL_MODEL(mod, ext) case ext##_EXT: debug("model is type " # ext); model->ptr = mod##_from_stream(stream)

    switch (model->header)
    {
        IMPL_MODEL(fmb,   FMB);
        IMPL_MODEL(fmb2, FMB2);

        default:
            debug("model is type UMB");
            stream->pos = 0;
            model->ptr = umb_from_stream(stream); //umb has no header!
    }

    #undef IMPL_MODEL

    stream_close(stream);

    return model;
}