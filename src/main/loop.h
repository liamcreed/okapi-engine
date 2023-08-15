#pragma once

vec3_t direction;

#define WINDOW_SIZE 800
f32 last_x = WINDOW_SIZE / 2;
f32 last_y = WINDOW_SIZE / 2;
f32 yaw = -90;
f32 pitch = 0;
f32 fov = 45;
bool first_mouse = true;

void init()
{
}

void update()
{
    f32 speed = 1 * window.dt;
    const f32 cam_speed = 1 * window.dt;

    f32 x_pos = window.mouse_pos.x;
    f32 y_pos = window.mouse_pos.y;

    if (first_mouse)
    {
        last_x = x_pos;
        last_y = y_pos;
        first_mouse = false;
    }

    f32 x_offset = x_pos - last_x;
    f32 y_offset = last_y - y_pos;
    last_x = x_pos;
    last_y = y_pos;

    const f32 sensitivity = 0.1f;
    x_offset *= sensitivity;
    y_offset *= sensitivity;

    yaw += x_offset;
    pitch += y_offset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    camera.front = vec3_normalize(direction);

    direction.x = cos(radians(yaw)) * cos(radians(pitch));
    direction.y = sin(radians(pitch));
    direction.z = sin(radians(yaw)) * cos(radians(pitch));

    if (key_pressed(&window, KEY_A))
    {
        camera.positition = vec3_subtract(camera.positition, vec3_scale(vec3_normalize(vec3_cross(camera.front, camera.up)), cam_speed));
    }
    if (key_pressed(&window, KEY_D))
    {
        camera.positition = vec3_add(camera.positition, vec3_scale(vec3_normalize(vec3_cross(camera.front, camera.up)), cam_speed));
    }
    if (key_pressed(&window, KEY_W))
    {
        camera.positition = vec3_add(camera.positition, vec3_scale(camera.front, cam_speed));
    }
    if (key_pressed(&window, KEY_S))
    {
        camera.positition = vec3_subtract(camera.positition, vec3_scale(camera.front, cam_speed));
    }
    if (key_pressed(&window, KEY_F))
        fov = 100;
    else
        fov = 45;
}




vec3_t player_pos = (vec3_t){0, 0, 2.5};
    f32 player_y_rot = 0;

    f32 last_x = window.size.x / 2;
    f32 last_y = window.size.y / 2;
    bool first_mouse = true;









f32 x_pos = window.mouse_pos.x;
f32 y_pos = window.mouse_pos.y;

if (first_mouse)
{
    last_x = x_pos;
    last_y = y_pos;
    first_mouse = false;
}

f32 x_offset = x_pos - last_x;
f32 y_offset = last_y - y_pos;
last_x = x_pos;
last_y = y_pos;

const f32 sensitivity = 0.1f;
x_offset *= sensitivity;
y_offset *= sensitivity;

camera.pitch += y_offset;
camera.yaw += x_offset;

static f32 radius = 5;
if (key_pressed(&window, KEY_UP))
    radius += 10 * window.dt;
else if (key_pressed(&window, KEY_DOWN))
    radius -= 10 * window.dt;

f32 x;
f32 z;
if (key_pressed(&window, KEY_A))
    x = -1;
else if (key_pressed(&window, KEY_D))
    x = 1;
else
    x = 0;
if (key_pressed(&window, KEY_W))
    z = -1;
else if (key_pressed(&window, KEY_S))
    z = 1;
else
    z = 0;

if (x != 0 && z != 0)
{
    x *= cos(radians(45));
    z *= cos(radians(45));
}

if (x != 0 || z != 0)
    player_y_rot = -camera.yaw;

f32 speed = radius * window.dt;
if (key_pressed(&window, KEY_LEFT_SHIFT))
    speed *= 2;

vec4_t cam_quat = quat_from_euler((vec3_t){0, -camera.yaw, 0});
vec3_t speed_vec = (vec3_t){x * speed, 0, z *speed};
vec3_t movement = vec3_multiply_quat(speed_vec, cam_quat);
player_pos = vec3_add(player_pos, movement);

f32 hor_dist = radius * cos(radians(camera.pitch));
f32 vert_dist = radius * sin(radians(camera.pitch));
f32 cam_x_offset = hor_dist * sin(radians(180 - camera.yaw));
f32 cam_z_offset = hor_dist * cos(radians(180 - camera.yaw));

camera.positition.x = player_pos.x - cam_x_offset;
camera.positition.y = lerp(camera.positition.y, player_pos.y - vert_dist, 0.8);
camera.positition.z = player_pos.z - cam_z_offset;