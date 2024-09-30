#include "umb.h"
#include <malloc.h>

UMB* umb_from_stream(Stream* stream)
{
    UMB* umb = (UMB*)malloc(sizeof(UMB));

    umb->numMaterials = stream_int(stream);
    umb->materials = ALLOC(UMBMaterial, umb->numMaterials);

    for (int i = 0; i < umb->numMaterials; i++)
    {
        umb->materials[i].name = stream_string(stream);
        umb->materials[i].texturePath = stream_string(stream);
        umb->materials[i].textureBase = stream_string(stream);

        OpaqueColor* ambient = STREAM_DATA(OpaqueColor);
        umb->materials[i].ambient = *ambient;

        free(ambient);

        OpaqueColor* diffuse = STREAM_DATA(OpaqueColor);
        umb->materials[i].diffuse = *diffuse;

        free(diffuse);

        OpaqueColor* specular = STREAM_DATA(OpaqueColor);
        umb->materials[i].specular = *specular;

        free(specular);

        umb->materials[i].glossiness = stream_float(stream);
    }

    umb->numObjects = stream_int(stream);
    umb->objects = ALLOC(UMBObject, umb->numObjects);

    for (int i = 0; i < umb->numObjects; i++)
    {
        UMBObject* object = &(umb->objects[i]);

        object->materialIndex = stream_int(stream);
        object->numKeyFrames = stream_int(stream);
        object->numAnimationFrames = stream_int(stream);

        object->frames = ALLOC(UMBFrame, object->numKeyFrames);

        for (int j = 0; j < object->numKeyFrames; j++)
        {
            UMBFrame* frame = &(object->frames[j]);

            frame->number = stream_int(stream);

            frame->usePreviousIndexData = stream_short_bool(stream);
            frame->usePreviousTextureData = stream_short_bool(stream);

            frame->numFaces = stream_int(stream);

            if (frame->numFaces > 0 && !frame->usePreviousIndexData)
            {
                frame->indices = STREAM_ARR(unsigned short, frame->numFaces * 3);
            }

            frame->numTextures = stream_int(stream);

            if (frame->numTextures > 0 && !frame->usePreviousTextureData)
            {
                frame->textures = STREAM_ARR(Vec2, frame->numTextures);
            }

            frame->numColors = stream_int(stream);

            if (frame->numColors > 0 && !frame->usePreviousTextureData)
            {
                frame->colors = STREAM_ARR(OpaqueColor32, frame->numColors);
            }

            frame->numVertices = stream_int(stream);

            if (frame->numVertices > 0)
            {
                frame->vertices = ALLOC(UMBVector3, frame->numVertices);
                frame->normals = ALLOC(UMBVector3, frame->numVertices);

                for (int k = 0; k < frame->numVertices; k++)
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

Vec3 umb_vector3_to_vector3(UMBVector3 vector)
{
    Vec3 vec3;

    vec3.x = vector.x;
    vec3.y = vector.y;
    vec3.z = -vector.z;
    
    return vec3;
}