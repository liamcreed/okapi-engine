#include "math/math.h"

mat4_t mat4_new(float value)
{
    mat4_t m;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (i == j)
                m.data[i][j] = value;
            else
                m.data[i][j] = 0;
        }
    }
    return m;
}
mat4_t mat4_ortho(float left, float right, float top, float bottom, float near, float far)
{
    mat4_t result;

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
mat4_t mat4_ortho_aspect(float aspect, float size, float near, float far)
{
    return mat4_ortho(-aspect * size, aspect * size, size, -size, near, far);
}

mat4_t mat4_perspective(float fov, float aspect, float near, float far)
{
    mat4_t result;
    float f = tan((radians(fov)) / 2);

    result.data[0][0] = 1 / (aspect * f);
    result.data[1][0] = 0;
    result.data[2][0] = 0;
    result.data[3][0] = 0;

    result.data[0][1] = 0;
    result.data[1][1] = 1 / f;
    result.data[2][1] = 0;
    result.data[3][1] = 0;

    float far_min_near = far - near;
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

mat4_t mat4_look_at(vec3_t eye, vec3_t center, vec3_t up)
{
    mat4_t result;
    vec3_t x, y, z;

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
mat4_t mat4_translate(mat4_t m, vec3_t vector)
{
    mat4_t result = m;
    result.data[0][3] = m.data[0][0] * vector.x + m.data[0][1] * vector.y + m.data[0][2] * vector.z + m.data[0][3];
    result.data[1][3] = m.data[1][0] * vector.x + m.data[1][1] * vector.y + m.data[1][2] * vector.z + m.data[1][3];
    result.data[2][3] = m.data[2][0] * vector.x + m.data[2][1] * vector.y + m.data[2][2] * vector.z + m.data[2][3];
    result.data[3][3] = m.data[3][0] * vector.x + m.data[3][1] * vector.y + m.data[3][2] * vector.z + m.data[3][3];
    return result;
}
mat4_t mat4_rotate_x(mat4_t m, float deg)
{
    quat_t q = quat_angle_axis(deg, (vec3_t){1,0,0});
    return mat4_multiply(m, mat4_from_quat(q));
}
mat4_t mat4_rotate_y(mat4_t m, float deg)
{
    quat_t q = quat_angle_axis(deg, (vec3_t){0,1,0});
    return mat4_multiply(m, mat4_from_quat(q));
}
mat4_t mat4_rotate_z(mat4_t m, float deg)
{
    quat_t q = quat_angle_axis(deg, (vec3_t){0,0,1});
    return mat4_multiply(m, mat4_from_quat(q));
}

mat4_t mat4_rotate(mat4_t m, vec3_t v)
{
    quat_t q = quat_from_euler(v);
    return mat4_multiply(m, mat4_from_quat(q));
}   



mat4_t mat4_scale(mat4_t m, float factor)
{
    mat4_t result = mat4_new(1);
    result.data[0][0] = factor;
    result.data[1][1] = factor;
    result.data[2][2] = factor;
    result = mat4_multiply(m, result);
    return result;
}
mat4_t mat4_multiply(mat4_t m1, mat4_t m2)
{
    mat4_t result = mat4_new(0);

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                result.data[i][j] += m1.data[i][k] * m2.data[k][j];
            }
        }
    }

    return result;
}
mat4_t mat4_inverse(mat4_t m)
{
    mat4_t result;
    
    return result;
    /* result.data[0][0] = m.data[0][0];
    result.data[0][1] = m.data[1][0];
    result.data[0][2] = m.data[2][0];
    result.data[0][3] = 0.0f;
    result.data[1][0] = m.data[0][1];
    result.data[1][1] = m.data[1][1];
    result.data[1][2] = m.data[2][1];
    result.data[1][3] = 0.0f;
    result.data[2][0] = m.data[0][2];
    result.data[2][1] = m.data[1][2];
    result.data[2][2] = m.data[2][2];
    result.data[2][3] = 0.0f;
    result.data[3][0] = -(m.data[3][0] * result.data[0][0] + m.data[3][1] * result.data[1][0] + m.data[3][2] * result.data[2][0]);
    result.data[3][1] = -(m.data[3][0] * result.data[0][1] + m.data[3][1] * result.data[1][1] + m.data[3][2] * result.data[2][1]);
    result.data[3][2] = -(m.data[3][0] * result.data[0][2] + m.data[3][1] * result.data[1][2] + m.data[3][2] * result.data[2][2]);
    result.data[3][3] = 1.0f; */
}

mat4_t mat4_from_quat(quat_t q)
{
    float xy = q.x * q.y;
    float xz = q.x * q.z;
    float xw = q.x * q.w;
    float yz = q.y * q.z;
    float yw = q.y * q.w;
    float zw = q.z * q.w;
    float xx = q.x * q.x;
    float yy = q.y * q.y;
    float zz = q.z * q.z;

    mat4_t result;
    /* result.data[0][0] = 1 - 2 * (yy + zz);
    result.data[1][0] = 2 * (xy - zw);
    result.data[2][0] = 2 * (xz + yw);
    result.data[3][0] = 0;

    result.data[0][1] = 2 * (xy + zw);
    result.data[1][1] = 1 - 2 * (xx + zz);
    result.data[2][1] = 2 * (yz - xw);
    result.data[3][1] = 0;

    result.data[0][2] = 2 * (xz - yw);
    result.data[1][2] = 2 * (yz + xw);
    result.data[2][2] = 1 - 2 * (xx + yy);
    result.data[3][2] = 0;

    result.data[0][3] = 0;
    result.data[1][3] = 0;
    result.data[2][3] = 0;
    result.data[3][3] = 1; */

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
mat4_t mat4_1D_to_2D(float* m)
{
    mat4_t result;
    /* result.data[0][0] = m[0];
    result.data[0][1] = m[1];
    result.data[0][2] = m[2];
    result.data[0][3] = m[3];

    result.data[1][0] = m[4];
    result.data[1][1] = m[5];
    result.data[1][2] = m[6];
    result.data[1][3] = m[7];

    result.data[2][0] = m[8];
    result.data[2][1] = m[9];
    result.data[2][2] = m[10];
    result.data[2][3] = m[11];

    result.data[3][0] = m[12];
    result.data[3][1] = m[13];
    result.data[3][2] = m[14];
    result.data[3][3] = m[15]; */

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

void mat4_print(mat4_t mat)
{
    printf("Matrix\n");
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            printf("%f ", mat.data[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
