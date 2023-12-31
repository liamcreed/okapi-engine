#include "math/math.h"

mat4 mat4_new(f32 value)
{
    mat4 m;
    memset(m.data, 0, sizeof(m.data));
    if (value)
    {
        for (u32 i = 0; i < 4; i++)
        {
            for (u32 j = 0; j < 4; j++)
            {
                if (i == j)
                    m.data[i][j] = value;
            }
        }
    }
    return m;
}

mat4 mat4_ortho(f32 left, f32 right, f32 top, f32 bottom, f32 near, f32 far)
{
    mat4 result;

    result.data[0][0] = 2 / (right - left);
    result.data[1][0] = 0;
    result.data[2][0] = 0;
    result.data[3][0] = 0;

    result.data[0][1] = 0;
    result.data[1][1] = 2 / (top - bottom);
    result.data[2][1] = 0;
    result.data[3][1] = 0;

    result.data[0][2] = 0;
    result.data[1][2] = 0;
    result.data[2][2] = -2 / (far - near);
    result.data[3][2] = 0;

    result.data[0][3] = -((right + left) / (right - left));
    result.data[1][3] = -((top + bottom) / (top - bottom));
    result.data[2][3] = -((far + near) / (far - near));
    result.data[3][3] = 1;

    return result;
}

mat4 mat4_ortho_aspect(f32 aspect, f32 size, f32 near, f32 far)
{
    return mat4_ortho(-aspect * size, aspect * size, size, -size, near, far);
}

mat4 mat4_perspective(f32 fov, f32 aspect, f32 near, f32 far)
{
    mat4 result;
    f32 tan_half_fov = tan(0.5f * deg_to_rad(fov));

    result.data[0][0] = 1 / (aspect * tan_half_fov);
    result.data[1][0] = 0;
    result.data[2][0] = 0;
    result.data[3][0] = 0;

    result.data[0][1] = 0;
    result.data[1][1] = 1 / tan_half_fov;
    result.data[2][1] = 0;
    result.data[3][1] = 0;

    f32 far_min_near = far - near;
    result.data[0][2] = 0;
    result.data[1][2] = 0;
    result.data[2][2] = -(far + near) / far_min_near;
    result.data[3][2] = -1;

    result.data[0][3] = 0;
    result.data[1][3] = 0;
    result.data[2][3] = -(2 * far * near) / far_min_near;
    result.data[3][3] = 0;

    return result;
}

mat4 mat4_look_at(vec3 eye, vec3 center, vec3 up)
{
    mat4 result;
    vec3 x, y, z;

    z = vec3_normalize(vec3_subtract(eye, center));
    y = up;
    x = vec3_normalize(vec3_cross(y, z));
    y = vec3_cross(z, x);
    x = vec3_normalize(x);
    y = vec3_normalize(y);

    result.data[0][0] = x.x;
    result.data[1][0] = y.x;
    result.data[2][0] = z.x;
    result.data[3][0] = 0;

    result.data[0][1] = x.y;
    result.data[1][1] = y.y;
    result.data[2][1] = z.y;
    result.data[3][1] = 0;

    result.data[0][2] = x.z;
    result.data[1][2] = y.z;
    result.data[2][2] = z.z;
    result.data[3][2] = 0;

    result.data[0][3] = -vec3_dot(x, eye);
    result.data[1][3] = -vec3_dot(y, eye);
    result.data[2][3] = -vec3_dot(z, eye);
    result.data[3][3] = 1.0f;
    return result;
}

mat4 mat4_translate(mat4 m, vec3 v)
{
    mat4 result = mat4_new(1);
    result.data[0][3] = v.x;
    result.data[1][3] = v.y;
    result.data[2][3] = v.z;
    result.data[3][3] = 1;
    return mat4_multiply(m, result);
}

mat4 mat4_rotate_x(mat4 m, f32 angle_deg)
{
    vec4 q = quat_angle_axis(angle_deg, (vec3) { 1, 0, 0 });
    return mat4_multiply(m, mat4_from_quat(q));
}

mat4 mat4_rotate_y(mat4 m, f32 angle_deg)
{
    vec4 q = quat_angle_axis(angle_deg, (vec3) { 0, 1, 0 });
    return mat4_multiply(m, mat4_from_quat(q));
}

mat4 mat4_rotate_z(mat4 m, f32 angle_deg)
{
    vec4 q = quat_angle_axis(angle_deg, (vec3) { 0, 0, 1 });
    return mat4_multiply(m, mat4_from_quat(q));
}

mat4 mat4_rotate_q(mat4 m, vec4 q)
{
    return mat4_multiply(m, mat4_from_quat(q));
}

mat4 mat4_rotate_e(mat4 m, vec3 v)
{
    vec4 q = quat_from_euler(v);
    return mat4_multiply(m, mat4_from_quat(q));
}

mat4 mat4_scale(mat4 m, vec3 size)
{
    mat4 result = mat4_new(1);
    result.data[0][0] = size.x;
    result.data[1][1] = size.y;
    result.data[2][2] = size.z;
    result = mat4_multiply(m, result);
    return result;
}

mat4 mat4_multiply(mat4 m1, mat4 m2)
{
    mat4 result = mat4_new(0);

    for (u32 i = 0; i < 4; i++)
    {
        for (u32 j = 0; j < 4; j++)
        {
            for (u32 k = 0; k < 4; k++)
            {
                result.data[i][j] += m1.data[i][k] * m2.data[k][j];
            }
        }
    }

    return result;

}

mat4 mat4_from_quat(vec4 q)
{
    f32 xy = q.x * q.y;
    f32 xz = q.x * q.z;
    f32 xw = q.x * q.w;
    f32 yz = q.y * q.z;
    f32 yw = q.y * q.w;
    f32 zw = q.z * q.w;
    f32 xx = q.x * q.x;
    f32 yy = q.y * q.y;
    f32 zz = q.z * q.z;

    mat4 result;

    result.data[0][0] = 1 - 2 * (yy + zz);
    result.data[0][1] = 2 * (xy - zw);
    result.data[0][2] = 2 * (xz + yw);
    result.data[0][3] = 0;

    result.data[1][0] = 2 * (xy + zw);
    result.data[1][1] = 1 - 2 * (xx + zz);
    result.data[1][2] = 2 * (yz - xw);
    result.data[1][3] = 0;

    result.data[2][0] = 2 * (xz - yw);
    result.data[2][1] = 2 * (yz + xw);
    result.data[2][2] = 1 - 2 * (xx + yy);
    result.data[2][3] = 0;

    result.data[3][0] = 0;
    result.data[3][1] = 0;
    result.data[3][2] = 0;
    result.data[3][3] = 1;
    return result;
}
mat4 mat4_1D_to_2D(f32* m)
{
    mat4 result;
    result.data[0][0] = m[0];
    result.data[1][0] = m[1];
    result.data[2][0] = m[2];
    result.data[3][0] = m[3];

    result.data[0][1] = m[4];
    result.data[1][1] = m[5];
    result.data[2][1] = m[6];
    result.data[3][1] = m[7];

    result.data[0][2] = m[8];
    result.data[1][2] = m[9];
    result.data[2][2] = m[10];
    result.data[3][2] = m[11];

    result.data[0][3] = m[12];
    result.data[1][3] = m[13];
    result.data[2][3] = m[14];
    result.data[3][3] = m[15];

    return result;
}

mat4 mat4_inverse(mat4 m)
{
    mat4 inverse;
    mat4 result;

    inverse.data[0][0] = m.data[1][1] * m.data[2][2] * m.data[3][3] -
        m.data[1][1] * m.data[3][2] * m.data[2][3] -
        m.data[1][2] * m.data[2][1] * m.data[3][3] +
        m.data[1][2] * m.data[3][1] * m.data[2][3] +
        m.data[1][3] * m.data[2][1] * m.data[3][2] -
        m.data[1][3] * m.data[3][1] * m.data[2][2];

    inverse.data[0][1] = -m.data[0][1] * m.data[2][2] * m.data[3][3] +
        m.data[0][1] * m.data[3][2] * m.data[2][3] +
        m.data[0][2] * m.data[2][1] * m.data[3][3] -
        m.data[0][2] * m.data[3][1] * m.data[2][3] -
        m.data[0][3] * m.data[2][1] * m.data[3][2] +
        m.data[0][3] * m.data[3][1] * m.data[2][2];

    inverse.data[0][2] = m.data[0][1] * m.data[1][2] * m.data[3][3] -
        m.data[0][1] * m.data[3][2] * m.data[1][3] -
        m.data[0][2] * m.data[1][1] * m.data[3][3] +
        m.data[0][2] * m.data[3][1] * m.data[1][3] +
        m.data[0][3] * m.data[1][1] * m.data[3][2] -
        m.data[0][3] * m.data[3][1] * m.data[1][2];

    inverse.data[0][3] = -m.data[0][1] * m.data[1][2] * m.data[2][3] +
        m.data[0][1] * m.data[2][2] * m.data[1][3] +
        m.data[0][2] * m.data[1][1] * m.data[2][3] -
        m.data[0][2] * m.data[2][1] * m.data[1][3] -
        m.data[0][3] * m.data[1][1] * m.data[2][2] +
        m.data[0][3] * m.data[2][1] * m.data[1][2];

    inverse.data[1][0] = -m.data[1][0] * m.data[2][2] * m.data[3][3] +
        m.data[1][0] * m.data[3][2] * m.data[2][3] +
        m.data[1][2] * m.data[2][0] * m.data[3][3] -
        m.data[1][2] * m.data[3][0] * m.data[2][3] -
        m.data[1][3] * m.data[2][0] * m.data[3][2] +
        m.data[1][3] * m.data[3][0] * m.data[2][2];

    inverse.data[1][1] = m.data[0][0] * m.data[2][2] * m.data[3][3] -
        m.data[0][0] * m.data[3][2] * m.data[2][3] -
        m.data[0][2] * m.data[2][0] * m.data[3][3] +
        m.data[0][2] * m.data[3][0] * m.data[2][3] +
        m.data[0][3] * m.data[2][0] * m.data[3][2] -
        m.data[0][3] * m.data[3][0] * m.data[2][2];

    inverse.data[1][2] = -m.data[0][0] * m.data[1][2] * m.data[3][3] +
        m.data[0][0] * m.data[3][2] * m.data[1][3] +
        m.data[0][2] * m.data[1][0] * m.data[3][3] -
        m.data[0][2] * m.data[3][0] * m.data[1][3] -
        m.data[0][3] * m.data[1][0] * m.data[3][2] +
        m.data[0][3] * m.data[3][0] * m.data[1][2];

    inverse.data[1][3] = m.data[0][0] * m.data[1][2] * m.data[2][3] -
        m.data[0][0] * m.data[2][2] * m.data[1][3] -
        m.data[0][2] * m.data[1][0] * m.data[2][3] +
        m.data[0][2] * m.data[2][0] * m.data[1][3] +
        m.data[0][3] * m.data[1][0] * m.data[2][2] -
        m.data[0][3] * m.data[2][0] * m.data[1][2];

    inverse.data[2][0] = m.data[1][0] * m.data[2][1] * m.data[3][3] -
        m.data[1][0] * m.data[3][1] * m.data[2][3] -
        m.data[1][1] * m.data[2][0] * m.data[3][3] +
        m.data[1][1] * m.data[3][0] * m.data[2][3] +
        m.data[1][3] * m.data[2][0] * m.data[3][1] -
        m.data[1][3] * m.data[3][0] * m.data[2][1];

    inverse.data[2][1] = -m.data[0][0] * m.data[2][1] * m.data[3][3] +
        m.data[0][0] * m.data[3][1] * m.data[2][3] +
        m.data[0][1] * m.data[2][0] * m.data[3][3] -
        m.data[0][1] * m.data[3][0] * m.data[2][3] -
        m.data[0][3] * m.data[2][0] * m.data[3][1] +
        m.data[0][3] * m.data[3][0] * m.data[2][1];

    inverse.data[2][2] = m.data[0][0] * m.data[1][1] * m.data[3][3] -
        m.data[0][0] * m.data[3][1] * m.data[1][3] -
        m.data[0][1] * m.data[1][0] * m.data[3][3] +
        m.data[0][1] * m.data[3][0] * m.data[1][3] +
        m.data[0][3] * m.data[1][0] * m.data[3][1] -
        m.data[0][3] * m.data[3][0] * m.data[1][1];

    inverse.data[2][3] = -m.data[0][0] * m.data[1][1] * m.data[2][3] +
        m.data[0][0] * m.data[2][1] * m.data[1][3] +
        m.data[0][1] * m.data[1][0] * m.data[2][3] -
        m.data[0][1] * m.data[2][0] * m.data[1][3] -
        m.data[0][3] * m.data[1][0] * m.data[2][1] +
        m.data[0][3] * m.data[2][0] * m.data[1][1];

    inverse.data[3][0] = -m.data[1][0] * m.data[2][1] * m.data[3][2] +
        m.data[1][0] * m.data[3][1] * m.data[2][2] +
        m.data[1][1] * m.data[2][0] * m.data[3][2] -
        m.data[1][1] * m.data[3][0] * m.data[2][2] -
        m.data[1][2] * m.data[2][0] * m.data[3][1] +
        m.data[1][2] * m.data[3][0] * m.data[2][1];

    inverse.data[3][1] = m.data[0][0] * m.data[2][1] * m.data[3][2] -
        m.data[0][0] * m.data[3][1] * m.data[2][2] -
        m.data[0][1] * m.data[2][0] * m.data[3][2] +
        m.data[0][1] * m.data[3][0] * m.data[2][2] +
        m.data[0][2] * m.data[2][0] * m.data[3][1] -
        m.data[0][2] * m.data[3][0] * m.data[2][1];

    inverse.data[3][2] = -m.data[0][0] * m.data[1][1] * m.data[3][2] +
        m.data[0][0] * m.data[3][1] * m.data[1][2] +
        m.data[0][1] * m.data[1][0] * m.data[3][2] -
        m.data[0][1] * m.data[3][0] * m.data[1][2] -
        m.data[0][2] * m.data[1][0] * m.data[3][1] +
        m.data[0][2] * m.data[3][0] * m.data[1][1];

    inverse.data[3][3] = m.data[0][0] * m.data[1][1] * m.data[2][2] -
        m.data[0][0] * m.data[2][1] * m.data[1][2] -
        m.data[0][1] * m.data[1][0] * m.data[2][2] +
        m.data[0][1] * m.data[2][0] * m.data[1][2] +
        m.data[0][2] * m.data[1][0] * m.data[2][1] -
        m.data[0][2] * m.data[2][0] * m.data[1][1];

    f32 det = m.data[0][0] * inverse.data[0][0] + m.data[1][0] * inverse.data[0][1] + m.data[2][0] * inverse.data[0][2] + m.data[3][0] * inverse.data[0][3];

    if (det == 0)
    {
        printf("[MATH]: no inverse found!\n");
        return mat4_new(1);
    }

    det = 1.0 / det;
    for (i32 i = 0; i < 4; i++)
    {
        for (i32 j = 0; j < 4; j++)
        {
            result.data[i][j] = inverse.data[i][j] * det;
        }

    }
    return result;
}

void mat4_print(mat4 mat)
{
    printf("Matrix\n");
    for (u32 i = 0; i < 4; i++)
    {
        for (u32 j = 0; j < 4; j++)
        {
            printf("%f ", mat.data[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}