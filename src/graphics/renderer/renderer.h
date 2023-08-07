#pragma once

#include "graphics/graphics.h"



typedef struct
{
    window_t* window;
    vec2_t resolution;

    vec4_t clear_color;

    mat4_t view_mat;
    mat4_t proj_mat;

    shader_t mesh_shader;
    shader_t anim_mesh_shader;
    shader_t quad_shader;
    shader_t pp_shader;

    framebuffer_t scene_buffer;
    framebuffer_t pp_buffer;
    vertex_array_t scene_buffer_va;

} renderer_t;

void renderer_create(renderer_t* renderer);
void renderer_exit(renderer_t *renderer);
void renderer_clear(renderer_t *renderer);

void renderer_start(renderer_t *renderer);
void renderer_end(renderer_t *renderer);

void renderer_batch_start(renderer_t *renderer);
void renderer_batch_end(renderer_t *renderer);

void renderer_draw_model_3D(renderer_t *renderer, camera_t* camera, model_3D_t *model, vec3_t pos, f32 size, vec4_t rotation);

void renderer_draw_quad(
    renderer_t *renderer,
    texture_t *texture,
    vec3_t pos,
    vec3_t size,
    vec4_t color);
