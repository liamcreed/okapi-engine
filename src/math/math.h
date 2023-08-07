#pragma once

typedef struct
{
    float x;
    float y;
    float z;
    float w;
}quat_t;

typedef struct
{
    float x;
    float y;
}vec2_t;

typedef struct
{
    float x;
    float y;
    float z;
}vec3_t;

typedef struct
{
    float x;
    float y;
    float z;
    float w;
}vec4_t;

typedef struct 
{
    float data[4][4];
} mat4_t;

float vec3_dot(vec3_t v1, vec3_t v2);
vec3_t vec3_normalize(vec3_t v);
vec3_t vec3_subtract(vec3_t v1, vec3_t v2);
vec3_t vec3_add(vec3_t v1, vec3_t v2);
vec3_t vec3_cross(vec3_t v1, vec3_t v2);
vec3_t vec3_scale(vec3_t v, float s);
void vec3_print(vec3_t vector);
vec3_t vec3_multiply_quat(vec3_t v, quat_t q);


quat_t quat_angle_axis(float angle, vec3_t axis);
quat_t quat_from_euler(vec3_t euler);
quat_t quat_normalize(quat_t q);
quat_t quat_conjugate(quat_t q);
quat_t quat_multiply(quat_t q1, quat_t q2);
quat_t quat_lerp(quat_t q1, quat_t q2, float f);
void quat_print(quat_t q);


mat4_t mat4_new(float value);
mat4_t mat4_ortho(float left, float right, float top, float bottom, float near, float far);
mat4_t mat4_ortho_aspect(float aspect, float size, float near, float far);
mat4_t mat4_perspective(float fov, float aspect, float near, float far);
mat4_t mat4_look_at(vec3_t eye, vec3_t center, vec3_t up);
mat4_t mat4_scale(mat4_t m, float factor);
mat4_t mat4_from_quat(quat_t q);
mat4_t mat4_rotate_x(mat4_t m, float deg);
mat4_t mat4_rotate_y(mat4_t m, float deg);
mat4_t mat4_rotate_z(mat4_t m, float deg);
mat4_t mat4_rotate(mat4_t m, vec3_t v);
mat4_t mat4_translate(mat4_t m, vec3_t vector);
mat4_t mat4_multiply(mat4_t m1, mat4_t m2);
mat4_t mat4_inverse(mat4_t m);
mat4_t mat4_1D_to_2D(float* m);
void mat4_print(mat4_t mat);



float lerp(float a, float b, float f);
float radians(float deg);