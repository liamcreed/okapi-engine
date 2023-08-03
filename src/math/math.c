#include "math/math.h"

quat_t quat_angle_axis(float angle,vec3_t axis)
{
    quat_t result;
    float half_angle = radians(angle/2);
    
    result.x = axis.x * sin(half_angle);
    result.y = axis.y * sin(half_angle);
    result.z = axis.z * sin(half_angle);
    result.w = cos(half_angle);
    
    return quat_normalize(result);
}
quat_t quat_from_euler(vec3_t euler)
{
    quat_t x = quat_angle_axis(euler.x, (vec3_t){1.0f, 0.0f, 0.0f});
    quat_t y = quat_angle_axis(euler.y, (vec3_t){0.0f, 1.0f, 0.0f});;
    quat_t z = quat_angle_axis(euler.z, (vec3_t){0.0f, 0.0f, 1.0f});;

    quat_t result = quat_multiply(x, y);
    return quat_multiply(quat_multiply(x, y), z);
}

quat_t quat_normalize(quat_t q)
{
    float lenght = sqrt((q.x * q.x) + (q.y * q.y) + (q.z * q.z) + (q.w * q.w));
    return (quat_t){q.x/lenght, q.y/lenght, q.z/lenght, q.w/lenght};
}
quat_t quat_conjugate(quat_t q)
{
    return (quat_t){-q.x, -q.y, -q.z, q.w};
}

quat_t quat_multiply(quat_t q1, quat_t q2)
{
    return (quat_t)
    {
        (q1.x * q2.w) + (q1.w * q2.x) + (q1.y * q2.z) - (q1.z * q2.y),  
        (q1.y * q2.w) + (q1.w * q2.y) + (q1.z * q2.x) - (q1.x * q2.z), 
        (q1.z * q2.w) + (q1.w * q2.z) + (q1.x * q2.y) - (q1.y * q2.x),
        (q1.w * q2.w) - (q1.x * q2.x) - (q1.y * q2.y) - (q1.z * q2.z),  
    };
}
void quat_print(quat_t q)
{
    printf("Quaternion: x: %f, y: %f, z: %f, w: %f\n", q.x, q.y, q.z, q.w);
}

quat_t quat_lerp(quat_t q1, quat_t q2, float f)
{
    quat_t result = (quat_t){0,0,0,1};
    return result;
}
float lerp(float a, float b, float f)
{
    return (a * (1.0 - f)) + (b * f);
}

float radians(float deg)
{
    return (float)(deg * (M_PI / 180.0));
}