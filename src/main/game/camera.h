
//---------------------------------------------------------------//

f32 x_input;
f32 z_input;
if (key_being_pressed(&window, KEY_A))
x_input = -1;
else if (key_being_pressed(&window, KEY_D))
x_input = 1;
else
x_input = 0;
if (key_being_pressed(&window, KEY_W))
z_input = -1;
else if (key_being_pressed(&window, KEY_S))
z_input = 1;
else
z_input = 0;

if (x_input != 0 && z_input != 0)
{
    x_input *= cos(deg_to_rad(45));
    z_input *= cos(deg_to_rad(45));
}

if (z_input == -1 && x_input == 0)
player.rotation = quat_lerp(player.rotation, quat_angle_axis(180, (vec3) { 0, 1, 0 }), window.dt * 20);
else if (z_input == 1 && x_input == 0)
player.rotation = quat_lerp(player.rotation, quat_angle_axis(0, (vec3) { 0, 1, 0 }), window.dt * 20);
else if (x_input == 1 && z_input == 0)
player.rotation = quat_lerp(player.rotation, quat_angle_axis(90, (vec3) { 0, 1, 0 }), window.dt * 20);
else if (x_input == -1 && z_input == 0)
player.rotation = quat_lerp(player.rotation, quat_angle_axis(270, (vec3) { 0, 1, 0 }), window.dt * 20);

else if (fabs(x_input - cos(deg_to_rad(45))) < 0.01 && fabs(z_input - cos(deg_to_rad(45))) < 0.01)
    player.rotation = quat_lerp(player.rotation, quat_angle_axis(45, (vec3) { 0, 1, 0 }), window.dt * 20);
else if (fabs(x_input + cos(deg_to_rad(45))) < 0.01 && fabs(z_input - cos(deg_to_rad(45))) < 0.01)
    player.rotation = quat_lerp(player.rotation, quat_angle_axis(-45, (vec3) { 0, 1, 0 }), window.dt * 20);
else if (fabs(x_input - cos(deg_to_rad(45))) < 0.01 && fabs(z_input + cos(deg_to_rad(45))) < 0.01)
    player.rotation = quat_lerp(player.rotation, quat_angle_axis(135, (vec3) { 0, 1, 0 }), window.dt * 20);
else if (fabs(x_input + cos(deg_to_rad(45))) < 0.01 && fabs(z_input + cos(deg_to_rad(45))) < 0.01)
    player.rotation = quat_lerp(player.rotation, quat_angle_axis(-135, (vec3) { 0, 1, 0 }), window.dt * 20);
 
f32 speed = 2 * window.dt;
if (key_being_pressed(&window, KEY_LEFT_SHIFT))
speed = 8 * window.dt;

vec3 speed_vec = (vec3){ x_input * speed, 0, z_input * speed };
player.position = vec3_add(player.position, speed_vec);


camera.positition.x = player.position.x;
camera.positition.y = player.position.y + 6;
camera.positition.z = player.position.z + 16;
