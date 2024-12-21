#include <umb.h>
#include <malloc.h>
#include <common.h>

UMB* umb_from_stream(Stream* stream)
{
    #define STREAM_VAL(v, t) umb->v = stream_##t(stream)

    UMB* umb = (UMB*)malloc(sizeof(UMB));

    STREAM_VAL(numMaterials, int);
    umb->materials = CALLOC(UMBMaterial, umb->numMaterials);

    FOREACH (i, umb->numMaterials)
    {
        #define NEXT_STRING(v) umb->materials[i].v = stream_string(stream)

        NEXT_STRING(name);
        NEXT_STRING(texturePath);
        NEXT_STRING(textureBase);

        #undef NEXT_STRING

        #define NEXT_COLOR(cl) OpaqueColor* cl = STREAM_DATA(OpaqueColor); umb->materials[i].cl = *cl; free(cl)

        NEXT_COLOR(ambient);
        NEXT_COLOR(diffuse);
        NEXT_COLOR(specular);

        #undef NEXT_COLOR

        umb->materials[i].glossiness = stream_float(stream);
    }

    STREAM_VAL(numObjects, int);
    umb->objects = CALLOC(UMBObject, umb->numObjects);

    FOREACH (i, umb->numObjects)
    {
        UMBObject* object = &(umb->objects[i]);
        #define NEXT_DATA(v, t) object->v = stream_##t(stream)

        NEXT_DATA(materialIndex, int);
        NEXT_DATA(numKeyFrames, int);
        NEXT_DATA(numAnimationFrames, int);

        object->frames = CALLOC(UMBFrame, object->numKeyFrames);

        FOREACH (j, object->numKeyFrames)
        {
            UMBFrame* frame = &(object->frames[j]);

            #define FRAME_DATA(v, t) frame->v = stream_##t(stream)

            FRAME_DATA(number, int);

            FRAME_DATA(usePreviousIndexData, short_bool);
            FRAME_DATA(usePreviousTextureData, short_bool);

            FRAME_DATA(numFaces, int);

            if (frame->numFaces > 0 && !frame->usePreviousIndexData)
            {
                frame->indices = STREAM_ARR(unsigned short, frame->numFaces * 3);
            }

            FRAME_DATA(numTextures, int);

            if (frame->numTextures > 0 && !frame->usePreviousTextureData)
            {
                frame->textures = STREAM_ARR(Vec2, frame->numTextures);
            }

            FRAME_DATA(numColors, int);

            if (frame->numColors > 0 && !frame->usePreviousTextureData)
            {
                frame->colors = STREAM_ARR(OpaqueColor32, frame->numColors);
            }

            FRAME_DATA(numVertices, int);

            if (frame->numVertices > 0)
            {
                frame->vertices = CALLOC(UMBVector3, frame->numVertices);
                frame->normals = CALLOC(UMBVector3, frame->numVertices);

                FOREACH (k, frame->numVertices)
                {
                    //why did you have to interleave the vertices and normals?? it could have been FASTER!!!!!

                    UMBVector3* vertex = STREAM_DATA(UMBVector3);
                    UMBVector3* normal = STREAM_DATA(UMBVector3);

                    frame->vertices[k] = *vertex;
                    frame->normals[k] = *normal;

                    free(vertex);
                    free(normal);
                }
            }

            #undef FRAME_DATA
        }

        #undef NEXT_DATA
    }

    #undef STREAM_VAL

    return umb;
}

void umb_frame_delete(UMBFrame frame)
{
    FREE(frame.indices);
    FREE(frame.textures);
    FREE(frame.colors);
    FREE(frame.vertices);
    FREE(frame.normals);
}

void umb_object_delete(UMBObject object)
{
    if (object.numKeyFrames > 0)
    {
        FOREACH (i, object.numKeyFrames)
        {
            umb_frame_delete(object.frames[i]);
        }

        free(object.frames);
    }
}

void umb_material_delete(UMBMaterial material)
{
    FREE(material.name);
    FREE(material.texturePath);
    FREE(material.textureBase);
}

void umb_delete(UMB* umb)
{
    if (umb != NULL)
    {
        if (umb->numMaterials > 0)
        {
            FOREACH (i, umb->numMaterials)
            {
                umb_material_delete(umb->materials[i]);
            }

            free(umb->materials);
        }

        if (umb->numObjects > 0)
        {
            FOREACH (i, umb->numObjects)
            {
                umb_object_delete(umb->objects[i]);
            }

            free(umb->objects);
        }

        free(umb);
    }
}

size_t umb_calc_size(UMB* umb)
{
    size_t size = UMB_SIZE;

    FOREACH (i, umb->numMaterials)
    {
        size += strlen(umb->materials[i].name) + 1;
        size += strlen(umb->materials[i].texturePath) + 1;
        size += strlen(umb->materials[i].textureBase) + 1;

        size += UMB_MATERIAL_SIZE;
    }

    FOREACH (i, umb->numObjects)
    {
        FOREACH (j, umb->objects[i].numKeyFrames)
        {
            if (!umb->objects[i].frames[j].usePreviousIndexData)
            {
                size += sizeof(unsigned short) * umb->objects[i].frames[j].numFaces;
            }

            if (!umb->objects[i].frames[j].usePreviousTextureData)
            {
                size += sizeof(Vec2) * umb->objects[i].frames[j].numTextures;
                size += sizeof(OpaqueColor32) * umb->objects[i].frames[j].numColors;
            }

            size += 24 /*vertex/normal pairs, better to not multiply it by two*/ * umb->objects[i].frames[j].numVertices;
            size += UMB_FRAME_SIZE;
        }

        size += UMB_OBJECT_SIZE;
    }

    return size;
}