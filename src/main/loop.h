#pragma once


vec3_t cam_pos = (vec3_t){0, 1, 3};
vec3_t cam_front = (vec3_t){0, 0, -1};
vec3_t cam_up = (vec3_t){0, 1, 0};
vec3_t direction;

#define WINDOW_SIZE 800
float last_x = WINDOW_SIZE / 2;
float last_y = WINDOW_SIZE / 2;
float yaw = -90;
float pitch = 0;
float fov = 45;
bool first_mouse = true;

void init()
{
   
}

void update()
{
    float speed = 1 * window.dt;
    const float cam_speed = 1 * window.dt;

    float x_pos = window.mouse_pos.x;
    float y_pos = window.mouse_pos.y;

    if (first_mouse) 
    {
        last_x = x_pos;
        last_y = y_pos;
        first_mouse = false;
    }

    float x_offset = x_pos - last_x;
    float y_offset = last_y - y_pos;
    last_x = x_pos;
    last_y = y_pos;

    const float sensitivity = 0.1f;
    x_offset *= sensitivity;
    y_offset *= sensitivity;

    yaw += x_offset;
    pitch += y_offset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    cam_front = vec3_normalize(direction);

    direction.x = cos(radians(yaw)) * cos(radians(pitch));
    direction.y = sin(radians(pitch));
    direction.z = sin(radians(yaw)) * cos(radians(pitch));

    if (key_pressed(&window, KEY_A))
    {
        cam_pos = vec3_subtract(cam_pos, vec3_scale(vec3_normalize(vec3_cross(cam_front, cam_up)), cam_speed));
    }
    if (key_pressed(&window, KEY_D))
    {
        cam_pos = vec3_add(cam_pos, vec3_scale(vec3_normalize(vec3_cross(cam_front, cam_up)), cam_speed));
    }
    if (key_pressed(&window, KEY_W))
    {
        cam_pos = vec3_add(cam_pos, vec3_scale(cam_front, cam_speed));
    }
    if (key_pressed(&window, KEY_S))
    {
        cam_pos = vec3_subtract(cam_pos, vec3_scale(cam_front, cam_speed));
    }
    if (key_pressed(&window, KEY_F))
        fov = 100;
    else
        fov = 45;
}