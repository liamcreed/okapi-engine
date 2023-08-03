#pragma once

#include "graphics/graphics.h"



typedef struct
{
    window_t* window;

    vec4_t clear_color;
    float resolution_scale;

    mat4_t view_mat;
    mat4_t proj_mat;

    shader_t mesh_shader;
    shader_t skybox_shader;
    shader_t quad_shader;

    framebuffer_t scene_buffer;
    vertex_array_t scene_buffer_va;
    shader_t pp_shader;

} renderer_t;

void renderer_create(renderer_t* renderer, window_t* window, vec4_t clear_color,  float resolution_scale);
void renderer_exit(renderer_t *renderer);
void renderer_clear(renderer_t *renderer);

void renderer_start(renderer_t *renderer);
void renderer_end(renderer_t *renderer);

void renderer_batch_start(renderer_t *renderer);
void renderer_batch_end(renderer_t *renderer);

void renderer_draw_model_3D(renderer_t *renderer, camera_t* camera, model_3D_t *model, vec3_t pos, float size, vec3_t rotation);

void renderer_draw_skybox(renderer_t *renderer, skybox_t* skybox);



void renderer_draw_sub_texture(
    renderer_t *renderer,
    texture_t *texture,
    sub_texture_t *sub_texture,
    vec3_t pos,
    vec3_t size,
    vec4_t color);

void renderer_draw_quad(
    renderer_t *renderer,
    texture_t *texture,
    vec3_t pos,
    vec3_t size,
    vec4_t color);
