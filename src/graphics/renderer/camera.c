#include "graphics/graphics.h"

void camera_create_perspective(camera_t* camera, float fov, float near, float far)
{
    camera->fov = fov;
    camera->near = near;
    camera->far = far;
    camera->orthographic = false;
    camera->orthographic_size = 1;
    camera->front = (vec3_t){0,0,-1};
    camera->up = (vec3_t){0,1,0};
    camera->yaw = -90;
}
void camera_create_orthographic(camera_t* camera, float orthograpic_size)
{
    camera->fov = 55;
    camera->near = -10;
    camera->far = 10;
    camera->orthographic = true;
    camera->orthographic_size = orthograpic_size;
}