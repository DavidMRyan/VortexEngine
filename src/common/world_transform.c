#include <stdio.h>
#include "../include/world_transform.h"

// Accessors
Matrix4f wt_get_matrix(WorldTransform* world) {
    Matrix4f scale;
    init_scale_transform(&scale, world->scale, world->scale, world->scale);

    Matrix4f rotation;
    rotation = init_rotation_transform(world->rotation.x, world->rotation.y, world->rotation.z);

    Matrix4f translation;
    init_translation_transform(&translation, world->position.x, world->position.y, world->position.z);

    Matrix4f temp = mul(&translation, &rotation);
    Matrix4f world_transform = mul(&temp, &scale);
    return world_transform;
}

// Mutators
void wt_set_position(WorldTransform* world, float x, float y, float z) {
    world->position.x = x;
    world->position.y = y;
    world->position.z = z;
}

void wt_set_rotation(WorldTransform* world, float x, float y, float z) {
    world->rotation.x = x;
    world->rotation.y = y;
    world->rotation.z = z;
}

void wt_set_scale(WorldTransform* world, float scale) {
    world->scale = scale;
}

// Additional
void wt_rotate(WorldTransform* world, float x, float y, float z) {
    world->rotation.x += x;
    world->rotation.y += y;
    world->rotation.z += z;
}