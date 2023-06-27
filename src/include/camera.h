#ifndef VORTEXENGINE_CAMERA_H
#define VORTEXENGINE_CAMERA_H

#include "math_3d.h"

typedef struct Camera {
    Vector3f position;
    Vector3f target;
    Vector3f up;
    float speed;
} Camera;

Camera new_camera();
Matrix4f cam_get_matrix(Camera* cam);
void init_camera_transform(Camera* cam, Matrix4f* _mat, const Vector3f* target, const Vector3f* up);

void cam_set_position(Camera* cam, float x, float y, float z);

void cam_keyboard_input(Camera* cam, unsigned char key);

#endif //VORTEXENGINE_CAMERA_H
