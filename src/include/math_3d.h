#ifndef VORTEXENGINE_MATH_3D_H
#define VORTEXENGINE_MATH_3D_H

#define radians(degrees) (float)(degrees * M_PI / 180.0f)
#define degrees(radians) (float)(radians * 180.0f / M_PI)
#define length(v)        (float)(sqrtf(v->x * v->x + v->y * v->y + v->z * v->z))

// ------------------------------------------------
// Vectors & Related

typedef struct Vector3f {
    float x;
    float y;
    float z;
} Vector3f;

Vector3f new_vec3f(float _x, float _y, float _z);
Vector3f vec3f_normalize(Vector3f* vec);
Vector3f vec3f_cross(Vector3f* vec, const Vector3f* v);
void svmul(Vector3f* v, float scalar);

// ------------------------------------------------



// ------------------------------------------------
// Matrices & Related

typedef struct Matrix4f {
    float mat[4][4];
} Matrix4f;

Matrix4f new_matrix4f(float a00, float a01, float a02, float a03,
                      float a10, float a11, float a12, float a13,
                      float a20, float a21, float a22, float a23,
                      float a30, float a31, float a32, float a33);
Matrix4f mul(const Matrix4f* left, const Matrix4f* right);
void init_scale_transform(Matrix4f* _mat, float x, float y, float z);
Matrix4f init_rotation_transform(float x, float y, float z);
//void init_rotation_qtransform(const Quaternion* quaternion); // TODO: Add Later
void init_translation_transform(Matrix4f* _mat, float x, float y, float z);
void init_camera_transform(Matrix4f* _mat, const Vector3f* target, const Vector3f* up);
void init_camera_transform_pos(Matrix4f* _mat, const Vector3f* pos, const Vector3f* target, const Vector3f* up);
//void init_perspective_transform(Matrix4f* _mat, const PersProjection* p); // TODO: Add Later
//void init_ortho_transform(Matrix4f* _mat, const OrthoProjection* p); // TODO: Add Later
void init_rotation_x(Matrix4f* _mat, float x);
void init_rotation_y(Matrix4f* _mat, float y);
void init_rotation_z(Matrix4f* _mat, float z);

// ------------------------------------------------



// ------------------------------------------------
// Vertices & Related

typedef struct Vertex {
    Vector3f pos;
    Vector3f color;
} Vertex;

Vertex new_vertex(float x, float y, float z);

// ------------------------------------------------

#endif //VORTEXENGINE_MATH_3D_H