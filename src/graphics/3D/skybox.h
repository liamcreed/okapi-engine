#pragma once

typedef struct 
{
    uint32_t id;
    vec2_t size;
    uint32_t channel_count;
}cubemap_t;

typedef struct 
{
    cubemap_t cubemap;
    vertex_array_t va;
    float* vertices;
}skybox_t;

void cubemap_load_from_files(cubemap_t* cubemap, const char** files, uint32_t count, bool filter);

void skybox_load_from_files(skybox_t* skybox, const char** files, bool filter);
