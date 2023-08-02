#pragma once

typedef struct
{
    float fov, near, far;

    vec3_t pos;
    vec3_t front;
    vec3_t up;

    float pitch;
    float yaw;
    float roll;

    bool orthographic;
    float orthographic_size;
} camera_t;

void camera_create_perspective(camera_t *camera, float fov, float near, float far);
void camera_create_orthographic(camera_t *camera, float orthograpic_size);