#pragma once

typedef struct
{
    u32 id;
    vec2_t size;
    u32 channel_count;
    unsigned char* data;
    bool filter, sRGB;
} texture_t;

void texture_create_from_png(texture_t *texture, const char *path);
void texture_export_to_bin(texture_t* texture, const char* path);
void texture_create_from_bin(texture_t *texture, const char *path);
void texture_create_from_data(texture_t *texture);
void texture_bind(texture_t *texture, u32 index);
void texture_unbind(u32 index);
void texture_delete(texture_t *texture);
void texture_update_data(texture_t *texture);
