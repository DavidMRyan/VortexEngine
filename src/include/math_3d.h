#ifndef VORTEXENGINE_MATH_3D_H
#define VORTEXENGINE_MATH_3D_H

typedef struct Vector3f {
    float x;
    float y;
    float z;
} Vector3f;

Vector3f new_vec3f(float _x, float _y, float _z) {
    Vector3f* _vec = malloc(sizeof(Vector3f));
    if(!_vec) fprintf(stderr, "Couldn't allocate size of Vector3f!\n");
    _vec->x = _x;
    _vec->y = _y;
    _vec->z = _z;

    return *_vec;
}

#endif //VORTEXENGINE_MATH_3D_H