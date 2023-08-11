#include "math/math.h"

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
    f32 length = sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
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


vec3_t vec3_multiply_mat4(vec3_t v, mat4_t m)
{
    return (vec3_t)
    {
        .x = v.x * m.data[0][0] + v.y * m.data[1][0] + v.z * m.data[2][0] + m.data[3][0],
            .y = v.x * m.data[0][1] + v.y * m.data[1][1] + v.z * m.data[2][1] + m.data[3][1],
            .z = v.x * m.data[0][2] + v.y * m.data[1][2] + v.z * m.data[2][2] + m.data[3][2]
    };
}

void vec3_print(vec3_t vector)
{
    printf("V3: x:%f y:%f z:%f\n", vector.x, vector.y, vector.z);
}