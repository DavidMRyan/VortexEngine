#ifndef VORTEXENGINE_PROJECTION_H
#define VORTEXENGINE_PROJECTION_H

#include "math_3d.h"

typedef struct PersProjection {
    float fov;
    int view_width;
    int view_height;
    float z_near;
    float z_far;
} PersProjection;

PersProjection new_pers_projection(float vfov, int w, int h, float near, float far);
Matrix4f persproj_get_matrix(PersProjection* proj);

#endif //VORTEXENGINE_PROJECTION_H
