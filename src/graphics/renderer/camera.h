#pragma once

typedef struct
{
    float fov, near, far;
    
    bool orthographic;
    float orthographic_size;
} camera_t;

void camera_create_perspective(camera_t *camera, float fov, float near, float far);
void camera_create_orthographic(camera_t *camera, float orthograpic_size);