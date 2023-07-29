#pragma once

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

mat4_t mat4_new(float value);
float vec3_dot(vec3_t v1, vec3_t v2);

vec3_t vec3_normalize(vec3_t v);
vec3_t vec3_subtract(vec3_t v1, vec3_t v2);
vec3_t vec3_add(vec3_t v1, vec3_t v2);
vec3_t vec3_cross(vec3_t v1, vec3_t v2);
vec3_t vec3_scale(vec3_t v, float s);

void vec3_print(vec3_t vector);
void mat4_print(mat4_t mat);

mat4_t mat4_ortho(float left, float right, float top, float bottom, float near, float far);
mat4_t mat4_ortho_aspect(float aspect, float size, float near, float far);
mat4_t mat4_perspective(float fov, float aspect, float near, float far);

mat4_t mat4_look_at(vec3_t eye, vec3_t center, vec3_t up);

mat4_t mat4_scale(mat4_t matrix, float factor);
mat4_t mat4_rotate_x(mat4_t matrix, float deg);
mat4_t mat4_rotate_y(mat4_t matrix, float deg);
mat4_t mat4_rotate_z(mat4_t matrix, float deg);
mat4_t mat4_translate(mat4_t matrix, vec3_t vector);

mat4_t mat4_multiply(mat4_t mat1, mat4_t mat2);
mat4_t mat4_inverse(mat4_t matrix);


float lerp(float a, float b, float f);
float radians(float deg);