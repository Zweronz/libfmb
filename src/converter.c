#include <converter.h>
#include <loader.h>

UMB* fmb_to_umb(FMB* fmb)
{
    //CREATE_MODEL(umb, UMB);
//
    //umb->numMaterials = fmb->numMaterials;
    //umb->materials = CALLOC(UMBMaterial, umb->numMaterials);
//
    //FOREACH (i, umb->numMaterials)
    //{
    //    COPY(umb, fmb, materials[i].ambient);
    //    COPY(umb, fmb, materials[i].diffuse);
    //    COPY(umb, fmb, materials[i].specular);
    //    COPY(umb, fmb, materials[i].glossiness);
//
    //    int len = strlen(fmb->materials[i].texturePath);
//
    //    umb->materials[i].texturePath = malloc(len);
    //    umb->materials[i].textureBase = malloc(len);
    //    
    //    memcpy(umb->materials[i].texturePath, fmb->materials[i].texturePath, len);
    //    memcpy(umb->materials[i].textureBase, umb->materials[i].texturePath, len);
    //    
    //    int nameLen = strlen(fmb->materials[i].name);
    //    umb->materials[i].name = malloc(nameLen);
//
    //    memcpy(umb->materials[i].name, fmb->materials[i].name, len);
    //}
//
    //umb->numObjects = fmb->numObjects;
    //umb->objects = CALLOC(UMBObject, umb->numObjects);
//
    //FOREACH(i, umb->numObjects)
    //{
    //    COPY(umb, fmb, objects[i].materialIndex);
    //    COPY(umb, fmb, objects[i].numKeyFrames);
    //    
    //    umb->objects[i].numAnimationFrames = fmb->numFrames;
    //}
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
        printf("source and destination are the same format! this is not allowed!\n");
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