#include "math/math.h"

f32 vec3_length(vec3_t v)
{
    return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

vec3_t vec3_add(vec3_t v1, vec3_t v2)
{
    return (vec3_t) { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

vec3_t vec3_subtract(vec3_t v1, vec3_t v2)
{
    return (vec3_t) { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

vec3_t vec3_cross(vec3_t v1, vec3_t v2)
{
    return (vec3_t) {
        v1.y* v2.z - v1.z * v2.y,
            v1.z* v2.x - v1.x * v2.z,
            v1.x* v2.y - v1.y * v2.x
    };
}

vec3_t vec3_scale(vec3_t v, f32 s)
{
    return (vec3_t) { v.x* s, v.y* s, v.z* s };
}

f32 vec3_dot(vec3_t v1, vec3_t v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

vec3_t vec3_normalize(vec3_t v)
{
    f32 length = vec3_length(v);
    return (vec3_t) { v.x / length, v.y / length, v.z / length };
}

vec3_t vec3_multiply_quat(vec3_t v, vec4_t q)
{
    vec3_t u = (vec3_t){ q.x, q.y, q.z };
    f32 s = q.w;

    vec3_t a = vec3_scale(u, 2.0f * vec3_dot(u, v));
    vec3_t b = vec3_scale(v, s * s - vec3_dot(u, u));
    vec3_t c = vec3_scale(vec3_cross(u, v), (2.0f * s));

    return vec3_add(vec3_add(a, b), c);
}

vec3_t vec3_multiply(vec3_t v1, vec3_t v2)
{
    return (vec3_t) {v1.x * v2.x, v1.y * v2.y, v1.z * v2.z};
}

vec3_t vec3_multiply_mat4(vec3_t v, mat4_t m)
{
    return (vec3_t)
    {
        .x = v.x * m.data[0][0] + v.y * m.data[1][0] + v.z * m.data[2][0] + m.data[3][0],
            .y = v.x * m.data[0][1] + v.y * m.data[1][1] + v.z * m.data[2][1] + m.data[3][1],
            .z = v.x * m.data[0][2] + v.y * m.data[1][2] + v.z * m.data[2][2] + m.data[3][2]
    };
}

vec3_t vec3_lerp(vec3_t v1, vec3_t v2, f32 f)
{
    vec3_t result;
    float blend_i = 1.0f - f;
    result.x = (v1.x * blend_i) + (v2.x * f);
    result.y = (v1.y * blend_i) + (v2.y * f);
    result.z = (v1.z * blend_i) + (v2.z * f);

    return result;
}

void vec3_print(vec3_t vector)
{
    printf("V3: x:%f y:%f z:%f\n", vector.x, vector.y, vector.z);
}

vec4_t quat_angle_axis(f32 angle, vec3_t axis)
{
    vec4_t result;
    f32 half_angle = radians(angle / 2);

    result.x = axis.x * sin(half_angle);
    result.y = axis.y * sin(half_angle);
    result.z = axis.z * sin(half_angle);
    result.w = cos(half_angle);

    return vec4_normalize(result);
}
vec4_t quat_from_euler(vec3_t euler)
{
    vec4_t x = quat_angle_axis(euler.x, (vec3_t) { 1.0f, 0.0f, 0.0f });
    vec4_t y = quat_angle_axis(euler.y, (vec3_t) { 0.0f, 1.0f, 0.0f });;
    vec4_t z = quat_angle_axis(euler.z, (vec3_t) { 0.0f, 0.0f, 1.0f });;

    vec4_t result = quat_multiply(x, y);
    return quat_multiply(quat_multiply(x, y), z);
}

f32 vec4_length(vec4_t v)
{
    return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z) + (v.w * v.w));
}

vec4_t vec4_normalize(vec4_t q)
{
    f32 length = vec4_length(q);
    return (vec4_t) { q.x / length, q.y / length, q.z / length, q.w / length };
}

vec4_t vec4_clamp(vec4_t v, vec4_t min, vec4_t max)
{
    vec4_t result = v;
    if (v.x < min.x)
        result.x = min.x;
    else  if (v.x > max.x)
        result.x = max.x;

    if (v.y < min.y)
        result.y = min.y;
    else if (v.y > max.y)
        result.y = max.y;

    if (v.z < min.z)
        result.z = min.z;
    else if (v.z > max.z)
        result.z = max.z;

    return result;
}

vec4_t quat_conjugate(vec4_t q)
{
    return (vec4_t) { -q.x, -q.y, -q.z, q.w };
}

vec4_t quat_multiply(vec4_t q1, vec4_t q2)
{
    return (vec4_t)
    {
        (q1.x * q2.w) + (q1.w * q2.x) + (q1.y * q2.z) - (q1.z * q2.y),
            (q1.y * q2.w) + (q1.w * q2.y) + (q1.z * q2.x) - (q1.x * q2.z),
            (q1.z * q2.w) + (q1.w * q2.z) + (q1.x * q2.y) - (q1.y * q2.x),
            (q1.w * q2.w) - (q1.x * q2.x) - (q1.y * q2.y) - (q1.z * q2.z),
    };
}

void vec4_print(vec4_t q)
{
    printf("Vec4: x: %f, y: %f, z: %f, w: %f\n", q.x, q.y, q.z, q.w);
}

float vec4_dot(vec4_t v1, vec4_t v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

vec4_t quat_lerp(vec4_t q1, vec4_t q2, f32 f)
{
    vec4_t result = (vec4_t){ 0,0,0,1 };
    float dot = vec4_dot(q1, q2);
    float blend_i = 1.0f - f;
    if (dot < 0)
    {
        result.x = (blend_i * q1.x) + (f * -q2.x);
        result.y = (blend_i * q1.y) + (f * -q2.y);
        result.z = (blend_i * q1.z) + (f * -q2.z);
        result.w = (blend_i * q1.w) + (f * -q2.w);
    }
    else
    {
        result.x = (blend_i * q1.x) + (f * q2.x);
        result.y = (blend_i * q1.y) + (f * q2.y);
        result.z = (blend_i * q1.z) + (f * q2.z);
        result.w = (blend_i * q1.w) + (f * q2.w);
    }
    return vec4_normalize(result);
}