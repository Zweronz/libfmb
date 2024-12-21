#include <fmb.h>
#include <common.h>

#define STREAM_VAL(v, t) fmb->v = stream_##t(stream)

FMBDataType num_to_data_type(int num)
{
    switch (num)
    {
        case 5120: return FMB_BYTE;
		case 5121: return FMB_UNSIGNED_BYTE;
		case 5122: return FMB_SHORT;
		case 5123: return FMB_UNSIGNED_SHORT;

		default: return FMB_FLOAT;
    }
}

int data_size(FMBDataType dataType)
{
    switch (dataType)
    {
        case FMB_BYTE:  case FMB_UNSIGNED_BYTE:   return 1;
        case FMB_SHORT: case FMB_UNSIGNED_SHORT:  return 2;
        
        default: return 4;
    }
}

FMB* fmb_from_stream(Stream* stream)
{
    FMB* fmb = (FMB*)malloc(sizeof(FMB));
    STREAM_VAL(version, float);

    #define NEXT_TYPE(s) fmb->s = data_size(num_to_data_type(stream_int(stream)))

    NEXT_TYPE(indexDataSize);  NEXT_TYPE(vertexDataSize);
    NEXT_TYPE(normalDataSize); NEXT_TYPE(textureDataSize);
    NEXT_TYPE(colorDataSize);

    #undef NEXT_TYPE

    STREAM_VAL(offset, float);
    STREAM_VAL(scale, float);

    fmb->inverseScale = 1.0f / fmb->scale;

    STREAM_VAL(numFrames, int);
    STREAM_VAL(numMaterials, int);

    fmb->materials = CALLOC(FMBMaterial, fmb->numMaterials);

    FOREACH (i, fmb->numMaterials)
    {
        FMBMaterial* material = &(fmb->materials[i]);

        material->name = stream_string(stream);
        material->texturePath = stream_string(stream);

        #define NEXT_COLOR(cl) OpaqueColor* cl = STREAM_DATA(OpaqueColor); material->cl = *cl; free(cl)

        NEXT_COLOR(ambient);
        NEXT_COLOR(diffuse);
        NEXT_COLOR(specular);

        #undef NEXT_COLOR

        material->glossiness = stream_float(stream);
    }

    STREAM_VAL(numObjects, int);
    fmb->objects = CALLOC(FMBObject, fmb->numObjects);

    FOREACH (i, fmb->numObjects)
    {
        FMBObject* object = &(fmb->objects[i]);

        #define NEXT_DATA(v, t) object->v = stream_##t(stream)

        NEXT_DATA(name, string);
        NEXT_DATA(materialIndex, int);

        NEXT_DATA(hasNormals, short_bool);
        NEXT_DATA(hasTextures, short_bool);
        NEXT_DATA(hasColors, short_bool);

        NEXT_DATA(numKeyFrames, int);
        object->frames = STREAM_ARR(FMBFrame, object->numKeyFrames);

        NEXT_DATA(numFaces, int);
        object->indices = STREAM_ARR(char, object->numFaces * fmb->indexDataSize * 3);
        
        FMBFrame* lastFrame = &(object->frames[object->numKeyFrames - 1]);
        NEXT_DATA(numVertices, int);

        object->vertices = STREAM_ARR(char, object->numVertices * 3 * fmb->vertexDataSize * (lastFrame->verticesOffset + 1));

        if (object->hasNormals)  object->normals  = STREAM_ARR(char, object->numVertices * 3 * fmb->normalDataSize * (lastFrame->verticesOffset + 1));
        if (object->hasTextures) object->textures = STREAM_ARR(char, object->numVertices * 2 * fmb->textureDataSize);
        if (object->hasColors)   object->colors   = STREAM_ARR(char, object->numVertices * 4 * fmb->colorDataSize);

        object->centers = STREAM_ARR(Vec3, object->numKeyFrames);
        object->radiuses = STREAM_ARR(float, object->numKeyFrames);

        object->keyFrameLookUp = STREAM_ARR(short, (fmb->numFrames + 1));

        #undef NEXT_DATA
    }

    fmb->mins = STREAM_ARR(Vec3, fmb->numFrames);
    fmb->maxes = STREAM_ARR(Vec3, fmb->numFrames);

    #undef STREAM_VAL

    return fmb;
}

void fmb_material_delete(FMBMaterial material)
{
    FREE(material.name);
    FREE(material.texturePath);
}

void fmb_object_delete(FMBObject object)
{
    FREE(object.name);
    FREE(object.frames);
    
    FREE(object.indices);
    FREE(object.vertices);
    FREE(object.normals);
    FREE(object.textures);
    FREE(object.colors);

    FREE(object.centers);
    FREE(object.radiuses);
    FREE(object.keyFrameLookUp);
}

void fmb_delete(FMB* fmb)
{
    if (fmb != NULL)
    {
        if (fmb->numMaterials > 0)
        {
            FOREACH (i, fmb->numMaterials)
            {
                fmb_material_delete(fmb->materials[i]);
            }

            free(fmb->materials);
        }

        if (fmb->numObjects > 0)
        {
            FOREACH (i, fmb->numObjects)
            {
                fmb_object_delete(fmb->objects[i]);
            }

            free(fmb->objects);
        }

        FREE(fmb->mins);
        FREE(fmb->maxes);
    }
}