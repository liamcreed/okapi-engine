#include "math/math.h"

vec4 quat_angle_axis(f32 angle, vec3 axis)
{
    vec4 result;
    f32 half_angle = deg_to_rad(angle / 2.0f);

    result.x = axis.x * sin(half_angle);
    result.y = axis.y * sin(half_angle);
    result.z = axis.z * sin(half_angle);
    result.w = cos(half_angle);

    return vec4_normalize(result);
}
vec4 quat_from_euler(vec3 euler)
{
    vec4 x = quat_angle_axis(euler.x, (vec3) { 1.0f, 0.0f, 0.0f });
    vec4 y = quat_angle_axis(euler.y, (vec3) { 0.0f, 1.0f, 0.0f });;
    vec4 z = quat_angle_axis(euler.z, (vec3) { 0.0f, 0.0f, 1.0f });;

    vec4 result = quat_multiply(x, y);
    return quat_multiply(quat_multiply(x, y), z);
}

vec4 quat_conjugate(vec4 q)
{
    return (vec4) { -q.x, -q.y, -q.z, q.w };
}

vec4 quat_multiply(vec4 q1, vec4 q2)
{
    return (vec4)
    {
        (q1.x * q2.w) + (q1.w * q2.x) + (q1.y * q2.z) - (q1.z * q2.y),
            (q1.y * q2.w) + (q1.w * q2.y) + (q1.z * q2.x) - (q1.x * q2.z),
            (q1.z * q2.w) + (q1.w * q2.z) + (q1.x * q2.y) - (q1.y * q2.x),
            (q1.w * q2.w) - (q1.x * q2.x) - (q1.y * q2.y) - (q1.z * q2.z),
    };
}

vec4 quat_lerp(vec4 q1, vec4 q2, f32 f)
{
    vec4 result = (vec4){ 0,0,0,1 };
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