#include "math/math.h"

vec4_t quat_angle_axis(f32 angle,vec3_t axis)
{
    vec4_t result;
    f32 half_angle = radians(angle/2);
    
    result.x = axis.x * sin(half_angle);
    result.y = axis.y * sin(half_angle);
    result.z = axis.z * sin(half_angle);
    result.w = cos(half_angle);
    
    return vec4_normalize(result);
}
vec4_t quat_from_euler(vec3_t euler)
{
    vec4_t x = quat_angle_axis(euler.x, (vec3_t){1.0f, 0.0f, 0.0f});
    vec4_t y = quat_angle_axis(euler.y, (vec3_t){0.0f, 1.0f, 0.0f});;
    vec4_t z = quat_angle_axis(euler.z, (vec3_t){0.0f, 0.0f, 1.0f});;

    vec4_t result = quat_multiply(x, y);
    return quat_multiply(quat_multiply(x, y), z);
}

vec4_t vec4_normalize(vec4_t q)
{
    f32 lenght = sqrt((q.x * q.x) + (q.y * q.y) + (q.z * q.z) + (q.w * q.w));
    return (vec4_t){q.x/lenght, q.y/lenght, q.z/lenght, q.w/lenght};
}
vec4_t quat_conjugate(vec4_t q)
{
    return (vec4_t){-q.x, -q.y, -q.z, q.w};
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
    printf("Quaternion: x: %f, y: %f, z: %f, w: %f\n", q.x, q.y, q.z, q.w);
}

float vec4_dot(vec4_t v1, vec4_t v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}
vec4_t quat_lerp(vec4_t q1, vec4_t q2, f32 f)
{
    vec4_t result = (vec4_t){0,0,0,1};
    float dot = vec4_dot(q1, q2);
    float blend_i = 1.0f - f;
    if(dot < 0)
    {
        result.x = (blend_i * q1.x) + (f * -q2.x);
        result.y = (blend_i * q1.y) + (f * -q2.y);
        result.z = (blend_i * q1.z) + (f * -q2.z);
        result.w = (blend_i * q1.w) + (f * -q2.w);
    }else
    {
        result.x = (blend_i * q1.x) + (f * q2.x);
        result.y = (blend_i * q1.y) + (f * q2.y);
        result.z = (blend_i * q1.z) + (f * q2.z);
        result.w = (blend_i * q1.w) + (f * q2.w);
    }
    return vec4_normalize(result);
}
f32 lerp(f32 a, f32 b, f32 f)
{
    return (a * (1.0 - f)) + (b * f);
}

f32 radians(f32 deg)
{
    return (f32)(deg * (M_PI / 180.0));
}