#include "math/math.h"

mat4_t mat4_new(float value)
{
    mat4_t matrix;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (i == j)
                matrix.data[i][j] = value;
            else
                matrix.data[i][j] = 0;
        }
    }
    return matrix;
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

    result.data[0][2] = 0;
    result.data[1][2] = 0;
    result.data[2][2] = -(far + near) / (far - near);
    result.data[3][2] = -1;

    result.data[0][3] = 0;
    result.data[1][3] = 0;
    result.data[2][3] = -(2 * far * near) / (far - near);
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
    result.data[3][2] = 0;
    result.data[1][2] = y.z;
    result.data[2][2] = z.z;

    result.data[0][3] = -vec3_dot(x, eye);
    result.data[1][3] = -vec3_dot(y, eye);
    result.data[2][3] = -vec3_dot(z, eye);
    result.data[3][3] = 1.0f;
    return result;
}
mat4_t mat4_translate(mat4_t matrix, vec3_t vector)
{
    mat4_t result = matrix;
    result.data[0][3] = matrix.data[0][0] * vector.x + matrix.data[0][1] * vector.y + matrix.data[0][2] * vector.z + matrix.data[0][3];
    result.data[1][3] = matrix.data[1][0] * vector.x + matrix.data[1][1] * vector.y + matrix.data[1][2] * vector.z + matrix.data[1][3];
    result.data[2][3] = matrix.data[2][0] * vector.x + matrix.data[2][1] * vector.y + matrix.data[2][2] * vector.z + matrix.data[2][3];
    result.data[3][3] = matrix.data[3][0] * vector.x + matrix.data[3][1] * vector.y + matrix.data[3][2] * vector.z + matrix.data[3][3];
    return result;
}
mat4_t mat4_rotate_x(mat4_t matrix, float deg)
{
    mat4_t result = mat4_new(1);
    float rad = radians(deg);

    result.data[1][1] = cos(rad);
    result.data[2][1] = sin(rad);
    result.data[1][2] = -sin(rad);
    result.data[2][2] = cos(rad);

    result = mat4_multiply(matrix, result);
    return result;
}
mat4_t mat4_rotate_y(mat4_t matrix, float deg)
{
    mat4_t result = mat4_new(1);
    float rad = radians(deg);

    result.data[0][0] = cos(rad);
    result.data[2][0] = sin(rad);
    result.data[0][2] = -sin(rad);
    result.data[2][2] = cos(rad);

    result = mat4_multiply(matrix, result);
    return result;
}
mat4_t mat4_rotate_z(mat4_t matrix, float deg)
{
    mat4_t result = mat4_new(1);
    float rad = radians(deg);

    result.data[0][0] = cos(rad);
    result.data[1][0] = sin(rad);
    result.data[0][1] = -sin(rad);
    result.data[1][1] = cos(rad);

    result = mat4_multiply(matrix, result);
    return result;
}
mat4_t mat4_scale(mat4_t matrix, float factor)
{
    mat4_t result = mat4_new(1);
    result.data[0][0] = factor;
    result.data[1][1] = factor;
    result.data[2][2] = factor;
    result = mat4_multiply(matrix, result);
    return result;
}
mat4_t mat4_multiply(mat4_t mat1, mat4_t mat2)
{
    mat4_t result;
    /* result.data[0][0] = mat1.data[0][0] * mat2.data[0][0] + mat1.data[1][0] * mat2.data[0][1] + mat1.data[2][0] * mat2.data[0][2] + mat1.data[3][0] * mat2.data[0][3];

    result.data[0][1] = mat1.data[0][1] * mat2.data[0][0] + mat1.data[1][1] * mat2.data[0][1] + mat1.data[2][1] * mat2.data[0][2] + mat1.data[3][1] * mat2.data[0][3];
    result.data[0][2] = mat1.data[0][2] * mat2.data[0][0] + mat1.data[1][2] * mat2.data[0][1] + mat1.data[2][2] * mat2.data[0][2] + mat1.data[3][2] * mat2.data[0][3];
    result.data[0][3] = mat1.data[0][3] * mat2.data[0][0] + mat1.data[1][3] * mat2.data[0][1] + mat1.data[2][3] * mat2.data[0][2] + mat1.data[3][3] * mat2.data[0][3];

    result.data[1][0] = mat1.data[0][0] * mat2.data[1][0] + mat1.data[1][0] * mat2.data[1][1] + mat1.data[2][0] * mat2.data[1][2] + mat1.data[3][0] * mat2.data[1][3];
    result.data[1][1] = mat1.data[0][1] * mat2.data[1][0] + mat1.data[1][1] * mat2.data[1][1] + mat1.data[2][1] * mat2.data[1][2] + mat1.data[3][1] * mat2.data[1][3];
    result.data[1][2] = mat1.data[0][2] * mat2.data[1][0] + mat1.data[1][2] * mat2.data[1][1] + mat1.data[2][2] * mat2.data[1][2] + mat1.data[3][2] * mat2.data[1][3];
    result.data[1][3] = mat1.data[0][3] * mat2.data[1][0] + mat1.data[1][3] * mat2.data[1][1] + mat1.data[2][3] * mat2.data[1][2] + mat1.data[3][3] * mat2.data[1][3];

    result.data[2][0] = mat1.data[0][0] * mat2.data[2][0] + mat1.data[1][0] * mat2.data[2][1] + mat1.data[2][0] * mat2.data[2][2] + mat1.data[3][0] * mat2.data[2][3];
    result.data[2][1] = mat1.data[0][1] * mat2.data[2][0] + mat1.data[1][1] * mat2.data[2][1] + mat1.data[2][1] * mat2.data[2][2] + mat1.data[3][1] * mat2.data[2][3];
    result.data[2][2] = mat1.data[0][2] * mat2.data[2][0] + mat1.data[1][2] * mat2.data[2][1] + mat1.data[2][2] * mat2.data[2][2] + mat1.data[3][2] * mat2.data[2][3];
    result.data[2][3] = mat1.data[0][3] * mat2.data[2][0] + mat1.data[1][3] * mat2.data[2][1] + mat1.data[2][3] * mat2.data[2][2] + mat1.data[3][3] * mat2.data[2][3];

    result.data[3][0] = mat1.data[0][0] * mat2.data[3][0] + mat1.data[1][0] * mat2.data[3][1] + mat1.data[2][0] * mat2.data[3][2] + mat1.data[3][0] * mat2.data[3][3];
    result.data[3][1] = mat1.data[0][1] * mat2.data[3][0] + mat1.data[1][1] * mat2.data[3][1] + mat1.data[2][1] * mat2.data[3][2] + mat1.data[3][1] * mat2.data[3][3];
    result.data[3][2] = mat1.data[0][2] * mat2.data[3][0] + mat1.data[1][2] * mat2.data[3][1] + mat1.data[2][2] * mat2.data[3][2] + mat1.data[3][2] * mat2.data[3][3];
    result.data[3][3] = mat1.data[0][3] * mat2.data[3][0] + mat1.data[1][3] * mat2.data[3][1] + mat1.data[2][3] * mat2.data[3][2] + mat1.data[3][3] * mat2.data[3][3]; */

    result.data[0][0] = mat1.data[0][0] * mat2.data[0][0] + mat1.data[0][1] * mat2.data[1][0] + mat1.data[0][2] * mat2.data[2][0] + mat1.data[0][3] * mat2.data[3][0];
    result.data[0][1] = mat1.data[0][0] * mat2.data[0][1] + mat1.data[0][1] * mat2.data[1][1] + mat1.data[0][2] * mat2.data[2][1] + mat1.data[0][3] * mat2.data[3][1];
    result.data[0][2] = mat1.data[0][0] * mat2.data[0][2] + mat1.data[0][1] * mat2.data[1][2] + mat1.data[0][2] * mat2.data[2][2] + mat1.data[0][3] * mat2.data[3][2];
    result.data[0][3] = mat1.data[0][0] * mat2.data[0][3] + mat1.data[0][1] * mat2.data[1][3] + mat1.data[0][2] * mat2.data[2][3] + mat1.data[0][3] * mat2.data[3][3];
    result.data[1][0] = mat1.data[1][0] * mat2.data[0][0] + mat1.data[1][1] * mat2.data[1][0] + mat1.data[1][2] * mat2.data[2][0] + mat1.data[1][3] * mat2.data[3][0];
    result.data[1][1] = mat1.data[1][0] * mat2.data[0][1] + mat1.data[1][1] * mat2.data[1][1] + mat1.data[1][2] * mat2.data[2][1] + mat1.data[1][3] * mat2.data[3][1];
    result.data[1][2] = mat1.data[1][0] * mat2.data[0][2] + mat1.data[1][1] * mat2.data[1][2] + mat1.data[1][2] * mat2.data[2][2] + mat1.data[1][3] * mat2.data[3][2];
    result.data[1][3] = mat1.data[1][0] * mat2.data[0][3] + mat1.data[1][1] * mat2.data[1][3] + mat1.data[1][2] * mat2.data[2][3] + mat1.data[1][3] * mat2.data[3][3];
    result.data[2][0] = mat1.data[2][0] * mat2.data[0][0] + mat1.data[2][1] * mat2.data[1][0] + mat1.data[2][2] * mat2.data[2][0] + mat1.data[2][3] * mat2.data[3][0];
    result.data[2][1] = mat1.data[2][0] * mat2.data[0][1] + mat1.data[2][1] * mat2.data[1][1] + mat1.data[2][2] * mat2.data[2][1] + mat1.data[2][3] * mat2.data[3][1];
    result.data[2][2] = mat1.data[2][0] * mat2.data[0][2] + mat1.data[2][1] * mat2.data[1][2] + mat1.data[2][2] * mat2.data[2][2] + mat1.data[2][3] * mat2.data[3][2];
    result.data[2][3] = mat1.data[2][0] * mat2.data[0][3] + mat1.data[2][1] * mat2.data[1][3] + mat1.data[2][2] * mat2.data[2][3] + mat1.data[2][3] * mat2.data[3][3];
    result.data[3][0] = mat1.data[3][0] * mat2.data[0][0] + mat1.data[3][1] * mat2.data[1][0] + mat1.data[3][2] * mat2.data[2][0] + mat1.data[3][3] * mat2.data[3][0];
    result.data[3][1] = mat1.data[3][0] * mat2.data[0][1] + mat1.data[3][1] * mat2.data[1][1] + mat1.data[3][2] * mat2.data[2][1] + mat1.data[3][3] * mat2.data[3][1];
    result.data[3][2] = mat1.data[3][0] * mat2.data[0][2] + mat1.data[3][1] * mat2.data[1][2] + mat1.data[3][2] * mat2.data[2][2] + mat1.data[3][3] * mat2.data[3][2];
    result.data[3][3] = mat1.data[3][0] * mat2.data[0][3] + mat1.data[3][1] * mat2.data[1][3] + mat1.data[3][2] * mat2.data[2][3] + mat1.data[3][3] * mat2.data[3][3];
 
    return result;
}
mat4_t mat4_inverse(mat4_t matrix)
{
    mat4_t result;
    result.data[0][0] = matrix.data[0][0];
    result.data[0][1] = matrix.data[1][0];
    result.data[0][2] = matrix.data[2][0];
    result.data[0][3] = 0.0f;
    result.data[1][0] = matrix.data[0][1];
    result.data[1][1] = matrix.data[1][1];
    result.data[1][2] = matrix.data[2][1];
    result.data[1][3] = 0.0f;
    result.data[2][0] = matrix.data[0][2];
    result.data[2][1] = matrix.data[1][2];
    result.data[2][2] = matrix.data[2][2];
    result.data[2][3] = 0.0f;
    result.data[3][0] = -(matrix.data[3][0] * result.data[0][0] + matrix.data[3][1] * result.data[1][0] + matrix.data[3][2] * result.data[2][0]);
    result.data[3][1] = -(matrix.data[3][0] * result.data[0][1] + matrix.data[3][1] * result.data[1][1] + matrix.data[3][2] * result.data[2][1]);
    result.data[3][2] = -(matrix.data[3][0] * result.data[0][2] + matrix.data[3][1] * result.data[1][2] + matrix.data[3][2] * result.data[2][2]);
    result.data[3][3] = 1.0f;
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
