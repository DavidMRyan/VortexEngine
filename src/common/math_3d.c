#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <assert.h>

#include "../include/math_3d.h"

// ------------------------------------------------
// Vector Definitions & Related

Vector3f new_vec3f(float _x, float _y, float _z) {
    Vector3f* _vec = malloc(sizeof(Vector3f));
    if(!_vec) fprintf(stderr, "Couldn't allocate size of Vector3f!\n");
    _vec->x = _x;
    _vec->y = _y;
    _vec->z = _z;

    return *_vec;
}

Vector3f vec3f_normalize(Vector3f* vec) {
    float len = length(vec);
    assert(len != 0);

    vec->x /= len;
    vec->y /= len;
    vec->z /= len;

    return *vec;
}

Vector3f vec3f_cross(Vector3f* vec, const Vector3f* v) {
    const float _x = vec->y * v->z - vec->z * v->y;
    const float _y = vec->z * v->x - vec->x * v->z;
    const float _z = vec->x * v->y - vec->y * v->x;

    return new_vec3f(_x, _y, _z);
}

// Scalar x Vector multiplication
void svmul(Vector3f* v, float scalar) {
    v->x *= scalar;
    v->y *= scalar;
    v->z *= scalar;
}

// ------------------------------------------------



// ------------------------------------------------
// Matrix Definitions & Related

Matrix4f new_matrix4f(float a00, float a01, float a02, float a03,
                      float a10, float a11, float a12, float a13,
                      float a20, float a21, float a22, float a23,
                      float a30, float a31, float a32, float a33) {
    Matrix4f* _mat = malloc(sizeof(Matrix4f));
    _mat->mat[0][0] = a00; _mat->mat[0][1] = a01; _mat->mat[0][2] = a02; _mat->mat[0][3] = a03;
    _mat->mat[1][0] = a10; _mat->mat[1][1] = a11; _mat->mat[1][2] = a12; _mat->mat[1][3] = a13;
    _mat->mat[2][0] = a20; _mat->mat[2][1] = a21; _mat->mat[2][2] = a22; _mat->mat[2][3] = a23;
    _mat->mat[3][0] = a30; _mat->mat[3][1] = a31; _mat->mat[3][2] = a32; _mat->mat[3][3] = a33;

    return *_mat;
}

Matrix4f mul(const Matrix4f* left, const Matrix4f* right) {
    Matrix4f* _mat = malloc(sizeof(Matrix4f));

    for (unsigned int i = 0; i < 4; i++) {
        for (unsigned int j = 0; j < 4; j++) {
            _mat->mat[i][j] = left->mat[i][0] * right->mat[0][j] +
                              left->mat[i][1] * right->mat[1][j] +
                              left->mat[i][2] * right->mat[2][j] +
                              left->mat[i][3] * right->mat[3][j];
        }
    }

    return *_mat;
}

// ------------------------------------------------



// ------------------------------------------------
// Vertices & Related

Vertex new_vertex(float x, float y, float z) {
    Vertex* vert = malloc(sizeof(Vertex));
    vert->pos = new_vec3f(x, y, z);

    float r = (float)rand() / (float)RAND_MAX;
    float g = (float)rand() / (float)RAND_MAX;
    float b = (float)rand() / (float)RAND_MAX;
    vert->color = new_vec3f(r, g, b);

    return *vert;
}

// ------------------------------------------------



void init_scale_transform(Matrix4f* _mat, float x, float y, float z) {
    _mat->mat[0][0] = x;    _mat->mat[0][1] = 0.0f; _mat->mat[0][2] = 0.0f; _mat->mat[0][3] = 0.0f;
    _mat->mat[1][0] = 0.0f; _mat->mat[1][1] = y;    _mat->mat[1][2] = 0.0f; _mat->mat[1][3] = 0.0f;
    _mat->mat[2][0] = 0.0f; _mat->mat[2][1] = 0.0f; _mat->mat[2][2] = z;    _mat->mat[2][3] = 0.0f;
    _mat->mat[3][0] = 0.0f; _mat->mat[3][1] = 0.0f; _mat->mat[3][2] = 0.0f; _mat->mat[3][3] = 1.0f;
}

void init_translation_transform(Matrix4f* _mat, float x, float y, float z) {
    _mat->mat[0][0] = 1.0f; _mat->mat[0][1] = 0.0f; _mat->mat[0][2] = 0.0f; _mat->mat[0][3] = x;
    _mat->mat[1][0] = 0.0f; _mat->mat[1][1] = 1.0f; _mat->mat[1][2] = 0.0f; _mat->mat[1][3] = y;
    _mat->mat[2][0] = 0.0f; _mat->mat[2][1] = 0.0f; _mat->mat[2][2] = 1.0f; _mat->mat[2][3] = z;
    _mat->mat[3][0] = 0.0f; _mat->mat[3][1] = 0.0f; _mat->mat[3][2] = 0.0f; _mat->mat[3][3] = 1.0f;
}

Matrix4f init_rotation_transform(float x, float y, float z) {
    Matrix4f rx, ry, rz;

    float _x = radians(x);
    float _y = radians(y);
    float _z = radians(z);

    init_rotation_x(&rx, _x);
    init_rotation_y(&ry, _y);
    init_rotation_z(&rz, _z);

    Matrix4f temp = mul(&rx, &ry);
    Matrix4f final = mul(&temp, &rz);
    return final;
}

void init_rotation_x(Matrix4f* _mat, float x) {
    _mat->mat[0][0] = 1.0f; _mat->mat[0][1] = 0.0f;     _mat->mat[0][2] = 0.0f;    _mat->mat[0][3] = 0.0f;
    _mat->mat[1][0] = 0.0f; _mat->mat[1][1] = cosf(x);  _mat->mat[1][2] = sinf(x); _mat->mat[1][3] = 0.0f;
    _mat->mat[2][0] = 0.0f; _mat->mat[2][1] = -sinf(x); _mat->mat[2][2] = cosf(x); _mat->mat[2][3] = 0.0f;
    _mat->mat[3][0] = 0.0f; _mat->mat[3][1] = 0.0f;     _mat->mat[3][2] = 0.0f;    _mat->mat[3][3] = 1.0f;
}

void init_rotation_y(Matrix4f* _mat, float y) {
    _mat->mat[0][0] = cosf(y); _mat->mat[0][1] = 0.0f; _mat->mat[0][2] = -sinf(y); _mat->mat[0][3] = 0.0f;
    _mat->mat[1][0] = 0.0f;       _mat->mat[1][1] = 1.0f; _mat->mat[1][2] = 0.0f;        _mat->mat[1][3] = 0.0f;
    _mat->mat[2][0] = sinf(y); _mat->mat[2][1] = 0.0f; _mat->mat[2][2] = cosf(y);  _mat->mat[2][3] = 0.0f;
    _mat->mat[3][0] = 0.0f;       _mat->mat[3][1] = 0.0f; _mat->mat[3][2] = 0.0f;        _mat->mat[3][3] = 1.0f;
}

void init_rotation_z(Matrix4f* _mat, float z) {
    _mat->mat[0][0] = cosf(z);  _mat->mat[0][1] = sinf(z); _mat->mat[0][2] = 0.0f; _mat->mat[0][3] = 0.0f;
    _mat->mat[1][0] = -sinf(z); _mat->mat[1][1] = cosf(z); _mat->mat[1][2] = 0.0f; _mat->mat[1][3] = 0.0f;
    _mat->mat[2][0] = 0.0f;        _mat->mat[2][1] = 0.0f;       _mat->mat[2][2] = 1.0f; _mat->mat[2][3] = 0.0f;
    _mat->mat[3][0] = 0.0f;        _mat->mat[3][1] = 0.0f;       _mat->mat[3][2] = 0.0f; _mat->mat[3][3] = 1.0f;
}

void init_camera_transform(Matrix4f* _mat, const Vector3f* target, const Vector3f* up) {
    Vector3f N = *target;
    vec3f_normalize(&N);

    Vector3f n_up = *up;
    vec3f_normalize(&n_up);

    Vector3f U = vec3f_cross(&n_up, &N);
    vec3f_normalize(&U);

    Vector3f V = vec3f_cross(&N, &U);

    _mat->mat[0][0] = U.x;   _mat->mat[0][1] = U.y;   _mat->mat[0][2] = U.z;   _mat->mat[0][3] = 0.0f;
    _mat->mat[1][0] = V.x;   _mat->mat[1][1] = V.y;   _mat->mat[1][2] = V.z;   _mat->mat[1][3] = 0.0f;
    _mat->mat[2][0] = N.x;   _mat->mat[2][1] = N.y;   _mat->mat[2][2] = N.z;   _mat->mat[2][3] = 0.0f;
    _mat->mat[3][0] = 0.0f;  _mat->mat[3][1] = 0.0f;  _mat->mat[3][2] = 0.0f;  _mat->mat[3][3] = 1.0f;
}