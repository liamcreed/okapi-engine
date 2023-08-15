#pragma once

typedef struct
{
    f32 fov, near, far;
    
    bool orthographic;
    f32 orthographic_size;

    vec3_t positition;
    vec4_t rotation;
} camera_t;
