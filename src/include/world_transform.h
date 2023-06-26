#ifndef VORTEXENGINE_WORLD_TRANSFORM_H
#define VORTEXENGINE_WORLD_TRANSFORM_H

#include "math_3d.h"

typedef struct WorldTransform {
    Vector3f position;
    Vector3f rotation;
    float scale;
} WorldTransform;

Matrix4f wt_get_matrix(WorldTransform* world);

void wt_set_position(WorldTransform* world, float x, float y, float z);
void wt_set_rotation(WorldTransform* world, float x, float y, float z);
void wt_set_scale(WorldTransform* world, float scale);

void wt_rotate(WorldTransform* world, float x, float y, float z);

#endif //VORTEXENGINE_WORLD_TRANSFORM_H
