#include "math/math.h"

f64 vec3_length_2(vec3 v1)
{
    return (v1.x * v1.x + v1.y * v1.y + v1.z * v1.z);
}

f32 vec3_length(vec3 v)
{
    return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

vec3 vec3_add(vec3 v1, vec3 v2)
{
    return (vec3) { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

vec3 vec3_subtract(vec3 v1, vec3 v2)
{
    return (vec3) { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

vec3 vec3_cross(vec3 v1, vec3 v2)
{
    return (vec3) {
        v1.y* v2.z - v1.z * v2.y,
            v1.z* v2.x - v1.x * v2.z,
            v1.x* v2.y - v1.y * v2.x
    };
}

vec3 vec3_multiply_f32(vec3 v, f32 s)
{
    return (vec3) { v.x* s, v.y* s, v.z* s };
}

f32 vec3_dot(vec3 v1, vec3 v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

vec3 vec3_normalize(vec3 v)
{
    f32 length = vec3_length(v);
    return (vec3) { v.x / length, v.y / length, v.z / length };
}

vec3 vec3_multiply_quat(vec3 v, vec4 q)
{
    vec3 u = (vec3){ q.x, q.y, q.z };
    f32 s = q.w;

    vec3 a = vec3_multiply_f32(u, 2.0f * vec3_dot(u, v));
    vec3 b = vec3_multiply_f32(v, s * s - vec3_dot(u, u));
    vec3 c = vec3_multiply_f32(vec3_cross(u, v), (2.0f * s));

    return vec3_add(vec3_add(a, b), c);
}

vec3 vec3_divide(vec3 v1, vec3 v2)
{
    return (vec3) { v1.x / v2.x, v1.y / v2.y, v1.z / v2.z };
}
vec3 vec3_divide_f32(vec3 v1, f32 f)
{
    return (vec3) { v1.x / f, v1.y / f, v1.z / f };
}
vec3 vec3_multiply(vec3 v1, vec3 v2)
{
    return (vec3) { v1.x* v2.x, v1.y* v2.y, v1.z* v2.z };
}

vec3 vec3_lerp(vec3 v1, vec3 v2, f32 f)
{
    vec3 result;
    float blend_i = 1.0f - f;
    result.x = (v1.x * blend_i) + (v2.x * f);
    result.y = (v1.y * blend_i) + (v2.y * f);
    result.z = (v1.z * blend_i) + (v2.z * f);

    return result;
}
vec3 vec3_clamp(vec3 v, vec3 min, vec3 max)
{
    vec3 result = v;
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
vec3 vec3_negate(vec3 v)
{
    return (vec3) { -v.x, -v.y, -v.z };
}
vec3 vec3_random_f32(f32 min, f32 max)
{
    return (vec3) { f32_random(min, max), f32_random(min, max), f32_random(min, max) };
}

vec3 vec3_reflect(vec3 v, vec3 n) 
{
    return vec3_subtract(v, vec3_multiply_f32(n, 2*vec3_dot(v,n)));
}

void vec3_print(vec3 vector)
{
    printf("V3: x:%f y:%f z:%f\n", vector.x, vector.y, vector.z);
}
