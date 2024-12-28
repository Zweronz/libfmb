#include <converter.h>
#include <loader.h>

UMB* fmb_to_umb(FMB* fmb)
{
    return NULL;
}

UMB* fmb2_to_umb(FMB2* fmb2)
{
    return NULL;
}

UMB* fma_to_umb(FMA* fma)
{
    return NULL;
}

FMB* umb_to_fmb(UMB* umb)
{
    return NULL;
}

FMB* fmb2_to_fmb(FMB2* fmb2)
{
    return NULL;
}

FMB* fma_to_fmb(FMA* fma)
{
    return NULL;
}

FMB2* umb_to_fmb2(UMB* umb)
{
    return NULL;
}

FMB2* fmb_to_fmb2(FMB* fmb)
{
    return NULL;
}

FMB2* fma_to_fmb2(FMA* fma)
{
    return NULL;
}

FMA* umb_to_fma(UMB* umb)
{
    return NULL;
}

FMA* fmb_to_fma(FMB* fmb)
{
    return NULL;
}

FMA* fmb2_to_fma(FMB2* fma)
{
    return NULL;
}

Model* convert_model(Model* model, int ext)
{
    if (model->header == ext)
    {
        debug("source and destination are the same format! this is not allowed!\n");
        return NULL;
    }

    Model* newModel = (Model*)malloc(sizeof(Model));
    newModel->header = ext;

    CONVERSION_GROUPS
    {
        GROUP(UMB)
        {
            ADD_CONVERSION(umb, fmb,  UMB,  FMB);
            ADD_CONVERSION(umb, fma,  UMB,  FMA);
            ADD_CONVERSION(umb, fmb2, UMB, FMB2);
        }

        GROUP(FMB)
        {
            ADD_CONVERSION(fmb, umb,  FMB,  UMB);
            ADD_CONVERSION(fmb, fma,  FMB,  FMA);
            ADD_CONVERSION(fmb, fmb2, FMB, FMB2);
        }

        GROUP(FMB2)
        {
            ADD_CONVERSION(fmb2, umb, FMB2, UMB);
            ADD_CONVERSION(fmb2, fmb, FMB2, FMB);
            ADD_CONVERSION(fmb2, fma, FMB2, FMA);
        }

        GROUP(FMA)
        {
            ADD_CONVERSION(fma, umb,  FMA,  UMB);
            ADD_CONVERSION(fma, fmb,  FMA,  FMB);
            ADD_CONVERSION(fma, fmb2, FMA, FMB2);
        }
    }

    return newModel;
}