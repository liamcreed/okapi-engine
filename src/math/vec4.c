#include "math/math.h"

f32 vec4_length(vec4 v)
{
    return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z) + (v.w * v.w));
}

vec4 vec4_normalize(vec4 q)
{
    f32 length = vec4_length(q);
    return (vec4) { q.x / length, q.y / length, q.z / length, q.w / length };
}

vec4 vec4_clamp(vec4 v, vec4 min, vec4 max)
{
    vec4 result = v;
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

void vec4_print(vec4 q)
{
    printf("Vec4: x: %f, y: %f, z: %f, w: %f\n", q.x, q.y, q.z, q.w);
}

float vec4_dot(vec4 v1, vec4 v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}
