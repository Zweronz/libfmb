#include <animation.h>

EXPORT void set_model_frame(Vec3* current, Vec3* a, Vec3* b, float t, int len)
{
    for (int i = 0; i < len; i++)
    {
        current->x = a->x + (b->x - a->x) * t;
        current->y = a->y + (b->y - a->y) * t;
        current->z = a->z + (b->z - a->z) * t;
    }
}