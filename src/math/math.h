#pragma once

typedef struct
{
    f32 x;
    f32 y;
}vec2_t;

typedef struct
{
    f32 x;
    f32 y;
    f32 z;
}vec3_t;

typedef struct
{
    f32 x;
    f32 y;
    f32 z;
    f32 w;
}vec4_t;

typedef struct 
{
    f32 data[4][4];
} mat4_t;

f32 vec3_dot(vec3_t v1, vec3_t v2);
vec3_t vec3_normalize(vec3_t v);
vec3_t vec3_subtract(vec3_t v1, vec3_t v2);
vec3_t vec3_add(vec3_t v1, vec3_t v2);
vec3_t vec3_cross(vec3_t v1, vec3_t v2);
vec3_t vec3_scale(vec3_t v, f32 s);
void vec3_print(vec3_t vector);
vec3_t vec3_multiply_quat(vec3_t v, vec4_t q);

vec3_t vec3_multiply_mat4(vec3_t v, mat4_t m);


vec4_t quat_angle_axis(f32 angle, vec3_t axis);
vec4_t quat_from_euler(vec3_t euler);
vec4_t vec4_normalize(vec4_t q);
vec4_t quat_conjugate(vec4_t q);
vec4_t quat_multiply(vec4_t q1, vec4_t q2);
vec4_t quat_lerp(vec4_t q1, vec4_t q2, f32 f);
void quat_print(vec4_t q);


mat4_t mat4_new(f32 value);
mat4_t mat4_ortho(f32 left, f32 right, f32 top, f32 bottom, f32 near, f32 far);
mat4_t mat4_ortho_aspect(f32 aspect, f32 size, f32 near, f32 far);
mat4_t mat4_perspective(f32 fov, f32 aspect, f32 near, f32 far);
mat4_t mat4_look_at(vec3_t eye, vec3_t center, vec3_t up);
mat4_t mat4_scale(mat4_t m, vec3_t size);
mat4_t mat4_from_quat(vec4_t q);
mat4_t mat4_rotate_x(mat4_t m, f32 deg);
mat4_t mat4_rotate_y(mat4_t m, f32 deg);
mat4_t mat4_rotate_z(mat4_t m, f32 deg);
mat4_t mat4_rotate_e(mat4_t m, vec3_t v);
mat4_t mat4_rotate_q(mat4_t m, vec4_t q);
mat4_t mat4_translate(mat4_t m, vec3_t vector);
mat4_t mat4_multiply(mat4_t m1, mat4_t m2);
mat4_t mat4_inverse(mat4_t m);
mat4_t mat4_1D_to_2D(f32* m);
void mat4_print(mat4_t mat);



f32 lerp(f32 a, f32 b, f32 f);
f32 radians(f32 deg);