
static float pitch, yaw;

static f32 last_x = 400;
static   f32 last_y = 400;

f32 x_pos = window.mouse_pos.x;
f32 y_pos = window.mouse_pos.y;


f32 x_offset = x_pos - last_x;
f32 y_offset = last_y - y_pos;
last_x = x_pos;
last_y = y_pos;

const f32 sensitivity = 0.1f;
x_offset *= sensitivity;
y_offset *= sensitivity;

pitch += y_offset;
yaw += x_offset;

static f32 radius = 10;
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
player.rotation.y = -yaw + 180;

f32 speed = window.dt;
if (key_pressed(&window, KEY_LEFT_SHIFT))
speed *= 2;

vec4_t cam_quat = quat_from_euler((vec3_t) { 0, -yaw, 0 });
vec3_t speed_vec = (vec3_t){ x * speed, 0, z * speed };
vec3_t movement = vec3_multiply_quat(speed_vec, cam_quat);
player.position = vec3_add(player.position, movement);

f32 hor_dist = radius * cos(radians(pitch));
f32 vert_dist = radius * sin(radians(pitch));
f32 cam_x_offset = hor_dist * sin(radians(180 - yaw));
f32 cam_z_offset = hor_dist * cos(radians(180 - yaw));

camera.pos.x = player.position.x - cam_x_offset;
camera.pos.y = lerp(player.position.y, player.position.y - vert_dist, 0.8);
camera.pos.z = player.position.z - cam_z_offset;
