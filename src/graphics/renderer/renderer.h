#pragma once

#include "graphics/graphics.h"


#define MAX_QUAD_COUNT 10000
#define MAX_TEXTURE_COUNT 16

typedef struct
{
    vec3_t pos;
    vec4_t color;
    vec2_t uv;
    float tex_index;
} quad_vertex_t;

typedef struct
{
    vec4_t clear_color;
    vec2_t resolution;

    mat4_t view_mat;
    mat4_t proj_mat;

    uint32_t quad_indices[MAX_QUAD_COUNT * 6];
    uint32_t quad_index_count;
    quad_vertex_t quad_vertices[MAX_QUAD_COUNT * 4];
    quad_vertex_t *quad_vertices_p;

    texture_t quad_textures[MAX_TEXTURE_COUNT];
    uint32_t quad_texture_count;

    shader_t quad_shader;
    vertex_array_t quad_vertex_array;


    shader_t model_3D_shader;
} renderer_t;

void renderer_create(renderer_t* renderer, vec4_t clear_color,  vec2_t resolution);
void renderer_init(renderer_t *renderer);
void renderer_exit(renderer_t *renderer);
void renderer_clear(renderer_t *renderer);

void renderer_start(renderer_t *renderer);
void renderer_end(renderer_t *renderer);

void renderer_draw_elements(renderer_t *renderer, uint32_t index_count);

void renderer_batch_start(renderer_t *renderer);
void renderer_batch_end(renderer_t *renderer);

void renderer_draw_model_3D(renderer_t *renderer, model_3D_t *model, vec3_t pos, float size, vec3_t rotation);




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
