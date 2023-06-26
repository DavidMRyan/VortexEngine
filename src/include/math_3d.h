#ifndef VORTEXENGINE_MATH_3D_H
#define VORTEXENGINE_MATH_3D_H

#define radians(degrees) (float)(degrees * M_PI / 180.0f)
#define degrees(radians) (float)(radians * 180.0f / M_PI)

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

typedef struct Matrix4f {
    float mat[4][4];
} Matrix4f;

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

typedef struct Vertex {
    Vector3f pos;
    Vector3f color;
} Vertex;

Vertex new_vertex(float x, float y, float z) {
    Vertex* vert = malloc(sizeof(Vertex));
    vert->pos = new_vec3f(x, y, z);

    float r = (float)rand() / (float)RAND_MAX;
    float g = (float)rand() / (float)RAND_MAX;
    float b = (float)rand() / (float)RAND_MAX;
    vert->color = new_vec3f(r, g, b);

    return *vert;
}

#endif //VORTEXENGINE_MATH_3D_H