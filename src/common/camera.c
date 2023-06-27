#include <stdio.h>
#include <malloc.h>

#include <GL/freeglut.h>

#include "../include/camera.h"

// Ctor
Camera new_camera() {
    Camera* cam = malloc(sizeof(Camera));
    cam->position = new_vec3f(0.0f, 0.0f, 0.0f);
    cam->target = new_vec3f(0.0f, 0.0f, 1.0f);
    cam->up = new_vec3f(0.0f, 1.0f, 0.0f);
    cam->speed = 1.0f;

    return *cam;
}

// Accessors
Matrix4f cam_get_matrix(Camera* cam) {
    Matrix4f cam_transform;
    init_camera_transform(&cam_transform, &cam->target, &cam->up);
    return cam_transform;
}

// Mutators
void cam_set_position(Camera* cam, float x, float y, float z) {
    cam->position.x = x;
    cam->position.y = y;
    cam->position.z = z;
}

// Additional
void cam_keyboard_input(Camera* cam, unsigned char key) {
    Vector3f left, right;
    switch(key) {
        case GLUT_KEY_UP:
            svmul(&cam->target, cam->speed);
            cam->position.x += cam->target.x;
            cam->position.y += cam->target.y;
            cam->position.z += cam->target.z;
            printf("[Up] Position: (%f, %f, [%f])\n", cam->position.x, cam->position.y, cam->position.z);
            break;

        case GLUT_KEY_DOWN:
            svmul(&cam->target, cam->speed);
            cam->position.x -= cam->target.x;
            cam->position.y -= cam->target.y;
            cam->position.z -= cam->target.z;
            printf("[Down] Position: (%f, %f, [%f])\n", cam->position.x, cam->position.y, cam->position.z);
            break;

        case GLUT_KEY_LEFT:
            left = vec3f_cross(&cam->target, &cam->up);
            vec3f_normalize(&left);
            svmul(&left, cam->speed);
            cam->position.x += left.x;
            cam->position.y += left.y;
            cam->position.z += left.z;
            printf("[Left] Position: ([%f], %f, %f)\n", cam->position.x, cam->position.y, cam->position.z);
            break;

        case GLUT_KEY_RIGHT:
            right = vec3f_cross(&cam->up, &cam->target);
            vec3f_normalize(&right);
            svmul(&right, cam->speed);
            cam->position.x += right.x;
            cam->position.y += right.y;
            cam->position.z += right.z;
            printf("[Right] Position: ([%f], %f, %f)\n", cam->position.x, cam->position.y, cam->position.z);
            break;

        case GLUT_KEY_PAGE_UP:
            cam->position.y += cam->speed;
            printf("[PgUp] Position: (%f, [%f], %f)\n", cam->position.x, cam->position.y, cam->position.z);
            break;

        case GLUT_KEY_PAGE_DOWN:
            cam->position.y -= cam->speed;
            printf("[PgDown] Position: (%f, [%f], %f)\n", cam->position.x, cam->position.y, cam->position.z);
            break;

        case '+':
            cam->speed += 0.1f;
            printf("[+] Speed: %f\n", cam->speed);
            break;

        case '-':
            cam->speed -= 0.1f;
            if(cam->speed < 0.1f) cam->speed = 0.1f;
            printf("[-] Speed: %f\n", cam->speed);
            break;

        default: break;
    }
}