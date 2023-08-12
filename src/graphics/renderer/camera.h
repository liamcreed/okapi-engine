#pragma once

typedef struct
{
    f32 fov, near, far;
    
    bool orthographic;
    f32 orthographic_size;
} camera_t;
/* 
void camera_create_perspective(camera_t *camera);
void camera_create_orthographic(camera_t *camera); */