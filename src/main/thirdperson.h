
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

//---------------------------------------------------------------//

f32 x_input;
f32 z_input;
if (key_pressed(&window, KEY_A))
x_input = -1;
else if (key_pressed(&window, KEY_D))
x_input = 1;
else
x_input = 0;
if (key_pressed(&window, KEY_W))
z_input = -1;
else if (key_pressed(&window, KEY_S))
z_input = 1;
else
z_input = 0;


if (x_input != 0 && z_input != 0)
{
    x_input *= cos(radians(45));
    z_input *= cos(radians(45));
}

if (z_input == -1 && x_input == 0)
player.rotation = quat_lerp(player.rotation, quat_angle_axis(-yaw + 180, (vec3_t) { 0, 1, 0 }), window.dt * 20);
else if (z_input == 1 && x_input == 0)
player.rotation = quat_lerp(player.rotation, quat_angle_axis(-yaw, (vec3_t) { 0, 1, 0 }), window.dt * 20);
else if (x_input == 1 && z_input == 0)
player.rotation = quat_lerp(player.rotation, quat_angle_axis(-yaw + 90, (vec3_t) { 0, 1, 0 }), window.dt * 20);
else if (x_input == -1 && z_input == 0)
player.rotation = quat_lerp(player.rotation, quat_angle_axis(-yaw + 270, (vec3_t) { 0, 1, 0 }), window.dt * 20);

else if (fabs(x_input - cos(radians(45))) < 0.01 && fabs(z_input - cos(radians(45))) < 0.01)
    player.rotation = quat_lerp(player.rotation, quat_angle_axis(-yaw + 45, (vec3_t) { 0, 1, 0 }), window.dt * 20);
else if (fabs(x_input + cos(radians(45))) < 0.01 && fabs(z_input - cos(radians(45))) < 0.01)
    player.rotation = quat_lerp(player.rotation, quat_angle_axis(-yaw - 45, (vec3_t) { 0, 1, 0 }), window.dt * 20);
else if (fabs(x_input - cos(radians(45))) < 0.01 && fabs(z_input + cos(radians(45))) < 0.01)
    player.rotation = quat_lerp(player.rotation, quat_angle_axis(-yaw + 135, (vec3_t) { 0, 1, 0 }), window.dt * 20);
else if (fabs(x_input + cos(radians(45))) < 0.01 && fabs(z_input + cos(radians(45))) < 0.01)
    player.rotation = quat_lerp(player.rotation, quat_angle_axis(-yaw - 135, (vec3_t) { 0, 1, 0 }), window.dt * 20);
 
f32 speed = 2 * window.dt;
if (key_pressed(&window, KEY_LEFT_SHIFT))
speed = 8 * window.dt;

vec4_t cam_quat = quat_from_euler((vec3_t) { 0, -yaw, 0 });
vec3_t speed_vec = (vec3_t){ x_input * speed, 0, z_input * speed };
vec3_t movement = vec3_multiply_quat(speed_vec, cam_quat);
player.position = vec3_add(player.position, movement);

//---------------------------------------------------------------//

f32 hor_dist = radius * cos(radians(pitch));
f32 vert_dist = radius * sin(radians(pitch));
f32 cam_x_offset = hor_dist * sin(radians(180 - yaw));
f32 cam_z_offset = hor_dist * cos(radians(180 - yaw));

camera.positition.x = player.position.x - cam_x_offset;
camera.positition.y = lerp(player.position.y, player.position.y - vert_dist, 0.8);
camera.positition.z = player.position.z - cam_z_offset;
