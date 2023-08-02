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


#include "math/vec.h"
#include "math/mat4.h"

quat_t quat_angle_axis(float angle, vec3_t axis);
quat_t quat_from_euler(vec3_t euler);
quat_t quat_normalize(quat_t quat);
quat_t quat_conjugate(quat_t quat);
quat_t quat_multiply(quat_t q1, quat_t q2);

void quat_print(quat_t q);

float lerp(float a, float b, float f);
float radians(float deg);