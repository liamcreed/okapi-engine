#pragma once

typedef struct
{
    u32 id;
    const char* v_path;
    const char* f_path;
    const char* g_path;
} shader_t;

void shader_create(shader_t* shader);
void shader_bind(shader_t *shader);
void shader_unbind();
void shader_delete(shader_t *shader);
void shader_set_uniform_mat4(shader_t *shader, const char *name, mat4 matrix);
void shader_set_uniform_mat4_arr(shader_t *shader, const char *name, mat4* matrices, u32 count);

void shader_set_uniform_int(shader_t *shader, const char *name, i32 data);
void shader_set_uniform_int_arr(shader_t *shader, const char *name, i32 *data, u32 count);
void shader_set_uniform_vec4(shader_t *shader, const char *name, vec4 data);
void shader_set_uniform_vec3(shader_t *shader, const char *name, vec3 data);
