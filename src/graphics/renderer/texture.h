#pragma once

typedef struct
{
    uint32_t id;
    vec2_t size;
    uint32_t channel_count;
} texture_t;

void texture_create_from_file(texture_t* texture,const char *path, bool filter, bool gamma);
void texture_create_from_data(texture_t* texture, unsigned char* data, vec2_t size, bool filter);
void texture_create_framebuffer_tex(texture_t* texture, vec2_t size, bool filter);
void texture_bind(texture_t *texture, uint32_t index);
void texture_unbind(uint32_t index);
void texture_delete(texture_t *texture);
void texture_update_data(texture_t *texture, unsigned char *data, vec2_t size);

typedef struct
{
    vec2_t uv[4];
    vec2_t tex_size;
    vec2_t sub_tex_size;
} sub_texture_t;

sub_texture_t sub_texture_create(vec2_t tex_size, vec2_t coords, vec2_t sub_tex_size);
