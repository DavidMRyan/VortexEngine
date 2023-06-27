#include <malloc.h>
#include <math.h>
#include <stdio.h>

#include "../include/projection.h"

PersProjection new_pers_projection(float vfov, int w, int h, float near, float far) {
    PersProjection* proj = malloc(sizeof(PersProjection));
    proj->fov = vfov;
    proj->view_width = w;
    proj->view_height = h;
    proj->z_near = near;
    proj->z_far = far;

    return *proj;
}

Matrix4f persproj_get_matrix(PersProjection* proj) {
    float tanHalfFOV = tanf(radians(proj->fov / 2.0f));
    float d = 1.0f / tanHalfFOV;
    float aspect = (float)proj->view_width / (float)proj->view_height;

    float z_range = proj->z_near - proj->z_far;
    float A = (-proj->z_far - proj->z_near) / z_range;
    float B = 2.0f * proj->z_far * proj->z_near / z_range;

    return new_matrix4f(d / aspect, 0.0f, 0.0f, 0.0f,
                        0.0f,       d,    0.0f, 0.0f,
                        0.0f,       0.0f, A,    B,
                        0.0f,       0.0f, 1.0f, 0.0f);
}