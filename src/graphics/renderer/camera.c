#include "graphics/graphics.h"

void camera_create_perspective(camera_t* camera)
{
    camera->orthographic = false;
    camera->orthographic_size = 1;
}
void camera_create_orthographic(camera_t* camera)
{
    camera->orthographic = true;
    camera->fov = 45;
}