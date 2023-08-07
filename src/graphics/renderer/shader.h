#pragma once

typedef struct
{
    uint32_t id;
} shader_t;

void shader_create(shader_t* shader,const char *vert_file_path, const char *frag_file_path);
void shader_bind(shader_t *shader);
void shader_unbind();
void shader_delete(shader_t *shader);
void shader_set_uniform_mat4(shader_t *shader, const char *name, mat4_t matrix);
void shader_set_uniform_mat4_arr(shader_t *shader, const char *name, mat4_t* matrices, uint32_t count);

void shader_set_uniform_int(shader_t *shader, const char *name, int data);
void shader_set_uniform_int_arr(shader_t *shader, const char *name, int *data, uint32_t count);
void shader_set_uniform_vec4(shader_t *shader, const char *name, vec4_t data);
void shader_set_uniform_vec3(shader_t *shader, const char *name, vec3_t data);
