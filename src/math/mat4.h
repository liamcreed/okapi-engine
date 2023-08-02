#pragma once

mat4_t mat4_new(float value);
mat4_t mat4_ortho(float left, float right, float top, float bottom, float near, float far);
mat4_t mat4_ortho_aspect(float aspect, float size, float near, float far);
mat4_t mat4_perspective(float fov, float aspect, float near, float far);

mat4_t mat4_look_at(vec3_t eye, vec3_t center, vec3_t up);

mat4_t mat4_scale(mat4_t matrix, float factor);
mat4_t mat4_rotate_x(mat4_t matrix, float deg);
mat4_t mat4_rotate_y(mat4_t matrix, float deg);
mat4_t mat4_rotate_z(mat4_t matrix, float deg);
mat4_t mat4_translate(mat4_t matrix, vec3_t vector);

mat4_t mat4_multiply(mat4_t mat1, mat4_t mat2);
mat4_t mat4_inverse(mat4_t matrix);

void mat4_print(mat4_t mat);