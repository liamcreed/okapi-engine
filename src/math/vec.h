#pragma once


float vec3_dot(vec3_t v1, vec3_t v2);

vec3_t vec3_normalize(vec3_t v);
vec3_t vec3_subtract(vec3_t v1, vec3_t v2);
vec3_t vec3_add(vec3_t v1, vec3_t v2);
vec3_t vec3_cross(vec3_t v1, vec3_t v2);
vec3_t vec3_scale(vec3_t v, float s);

void vec3_print(vec3_t vector);

vec3_t vec3_multiply_quat(vec3_t v, quat_t q);

