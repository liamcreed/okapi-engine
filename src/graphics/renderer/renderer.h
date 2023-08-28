#pragma once

#include "graphics/graphics.h"

typedef struct
{
    window_t* window;
    u32 width;
    u32 height;

    vec4 clear_color;

    mat4 view_mat;
    mat4 proj_mat;

    shader_t mesh_shader;

    shader_t quad_shader;
    vertex_array_t quad_va;
    texture_t white_tex;

    framebuffer_t scene_buffer;
    shader_t pp_shader;
    vertex_array_t scene_buffer_va;

} renderer_t;

void renderer_create(renderer_t* renderer);
void renderer_exit(renderer_t *renderer);
void renderer_clear(renderer_t *renderer);

void renderer_start(renderer_t *renderer);
void renderer_end(renderer_t *renderer);

void renderer_draw_model_3D(renderer_t *renderer, model_3D_t *model, vec3 position, f32 size, vec4 rotation);
void renderer_draw_quad(renderer_t* renderer, texture_t* tex, vec4 color, vec3 position, vec3 size, vec4 rotation);
