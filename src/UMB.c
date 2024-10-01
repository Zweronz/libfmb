#include <umb.h>
#include <malloc.h>
#include <common.h>

UMB* umb_from_stream(Stream* stream)
{
    UMB* umb = ALLOC_DATA(UMB);

    STREAM_VAL(numMaterials, int);
    umb->materials = ALLOC_ARR(UMBMaterial, umb->numMaterials);

    FOR (umb->numMaterials)
    {
        #define NEXT_STRING(v) umb->materials[i].v = stream_string(stream);

        NEXT_STRING(name);
        NEXT_STRING(texturePath);
        NEXT_STRING(textureBase);

        #define NEXT_COLOR(cl) OpaqueColor* cl = STREAM_DATA(OpaqueColor); umb->materials[i].cl = *cl; free(cl)

        NEXT_COLOR(ambient);
        NEXT_COLOR(diffuse);
        NEXT_COLOR(specular);

        umb->materials[i].glossiness = stream_float(stream);
    }

    STREAM_VAL(numObjects, int);
    umb->objects = ALLOC_ARR(UMBObject, umb->numObjects);

    FOR (umb->numObjects)
    {
        UMBObject* object = &(umb->objects[i]);
        #define NEXT_DATA(v, t) object->v = stream_##t(stream)

        NEXT_DATA(materialIndex, int);
        NEXT_DATA(numKeyFrames, int);
        NEXT_DATA(numAnimationFrames, int);

        object->frames = ALLOC_ARR(UMBFrame, object->numKeyFrames);

        FOR_N (j, object->numKeyFrames)
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
                frame->vertices = ALLOC_ARR(UMBVector3, frame->numVertices);
                frame->normals = ALLOC_ARR(UMBVector3, frame->numVertices);

                FOR_N (k, frame->numVertices)
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
        }
    }

    printf("finished reading\n");

    return umb;
}

void umb_frame_delete(UMBFrame frame)
{
    if (frame.numFaces > 0)
    {
        free(frame.indices);
    }

    if (frame.numTextures > 0)
    {
        free(frame.textures);
    }

    if (frame.numColors > 0)
    {
        free(frame.colors);
    }

    if (frame.numVertices > 0)
    {
        free(frame.vertices);
        free(frame.normals);
    }
}

void umb_object_delete(UMBObject object)
{
    if (object.numKeyFrames > 0)
    {
        for (int i = 0; i < object.numKeyFrames; i++)
        {
            umb_frame_delete(object.frames[i]);
        }

        free(object.frames);
    }
}

void umb_material_delete(UMBMaterial material)
{
    if (material.name != NULL)
    {
        free(material.name);
    }

    if (material.texturePath != NULL)
    {
        free(material.texturePath);
    }

    if (material.textureBase != NULL)
    {
        free(material.textureBase);
    }
}

void umb_delete(UMB* umb)
{
    if (umb != NULL)
    {
        if (umb->numMaterials > 0)
        {
            for (int i = 0; i < umb->numMaterials; i++)
            {
                umb_material_delete(umb->materials[i]);
            }

            free(umb->materials);
        }

        if (umb->numObjects > 0)
        {
            for (int i = 0; i < umb->numObjects; i++)
            {
                umb_object_delete(umb->objects[i]);
            }

            free(umb->objects);
        }

        free(umb);
    }
}