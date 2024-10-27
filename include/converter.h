#pragma once

#include <umb.h>
#include <fmb.h>
#include <fmb2.h>
#include <fma.h>
#include <loader.h>
#include <malloc.h>

#define ADD_CONVERSION(f, t, e, s) case s##_EXT: newModel->ptr = f##_to_##t((e*)model->ptr)
#define GROUP(e) case e##_EXT: switch (newModel->header)

#define CONVERSION_GROUPS switch (model->header) //this is completely useless but it's strictly to make the conversion group section look nice

#define CREATE_MODEL(v, t) t* v = (t*)malloc(sizeof(t)) //also useless, but it makes the conversion code less terrible to read
#define COPY(s, d, v) s->##v = d->##v //a lot of types are similar so this is just nice to work with

UMB* fmb_to_umb(FMB* fmb);

UMB* fmb2_to_umb(FMB2* fmb2);

UMB* fma_to_umb(FMA* fma);

FMB* umb_to_fmb(UMB* umb);

FMB* fmb2_to_fmb(FMB2* fmb2);

FMB* fma_to_fmb(FMA* fma);

FMB2* umb_to_fmb2(UMB* umb);

FMB2* fmb_to_fmb2(FMB* fmb);

FMB2* fma_to_fmb2(FMA* fma);

FMA* umb_to_fma(UMB* umb);

FMA* fmb_to_fma(FMB* fmb);

FMA* fmb2_to_fma(FMB2* fma);

Model* convert_model(Model* model, int ext);