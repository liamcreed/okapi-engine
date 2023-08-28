#pragma once

typedef struct
{
    f32 x;
    f32 y;
}vec2;

typedef struct
{
    f32 x;
    f32 y;
    f32 z;
}vec3;

typedef struct
{
    f32 x;
    f32 y;
    f32 z;
    f32 w;
}vec4;

typedef struct 
{
    f32 data[4][4];
} mat4;


f32 vec3_dot(vec3 v1, vec3 v2);
f32 vec3_length(vec3 v);
vec3 vec3_normalize(vec3 v);

vec3 vec3_subtract(vec3 v1, vec3 v2);
vec3 vec3_add(vec3 v1, vec3 v2);
vec3 vec3_multiply(vec3 v1, vec3 v2);
vec3 vec3_cross(vec3 v1, vec3 v2);
vec3 vec3_scale(vec3 v, f32 s);
vec3 vec3_divide(vec3 v1, vec3 v2);
vec3 vec3_divide_f32(vec3 v1, f32 f);
vec3 vec3_multiply_quat(vec3 v, vec4 q);
vec3 vec3_lerp(vec3 v1, vec3 v2, f32 f);
vec3 vec3_clamp(vec3 v, vec3 min, vec3 max);
vec3 vec3_negate(vec3 v);
vec3 vec3_random_f32(f32 min, f32 max);
vec3 vec3_reflect(vec3 v, vec3 n);
void vec3_print(vec3 vector);

vec4 vec4_normalize(vec4 q);
vec4 vec4_clamp(vec4 v, vec4 min, vec4 max);
float vec4_dot(vec4 v1, vec4 v2);
void vec4_print(vec4 q);

vec4 quat_angle_axis(f32 angle, vec3 axis);
vec4 quat_from_euler(vec3 euler);
vec4 quat_conjugate(vec4 q);
vec4 quat_multiply(vec4 q1, vec4 q2);
vec4 quat_lerp(vec4 q1, vec4 q2, f32 f);


mat4 mat4_new(f32 value);
mat4 mat4_ortho(f32 left, f32 right, f32 top, f32 bottom, f32 near, f32 far);
mat4 mat4_ortho_aspect(f32 aspect, f32 size, f32 near, f32 far);
mat4 mat4_perspective(f32 fov, f32 aspect, f32 near, f32 far);
mat4 mat4_look_at(vec3 eye, vec3 center, vec3 up);
mat4 mat4_scale(mat4 m, vec3 size);
mat4 mat4_from_quat(vec4 q);
mat4 mat4_rotate_x(mat4 m, f32 deg);
mat4 mat4_rotate_y(mat4 m, f32 deg);
mat4 mat4_rotate_z(mat4 m, f32 deg);
mat4 mat4_rotate_e(mat4 m, vec3 v);
mat4 mat4_rotate_q(mat4 m, vec4 q);
mat4 mat4_translate(mat4 m, vec3 vector);
mat4 mat4_multiply(mat4 m1, mat4 m2);
mat4 mat4_inverse(mat4 m);
mat4 mat4_1D_to_2D(f32* m);
void mat4_print(mat4 mat);

f32 lerp(f32 a, f32 b, f32 f);
f32 deg_to_rad(f32 deg);

f32 f32_max(f32 f1, f32 f2);
f32 f32_random(f32 min, f32 max);
f32 f32_clamp(f32 f, f32 min, f32 max);