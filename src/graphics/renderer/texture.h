#pragma once

typedef struct
{
    uint32_t id;
    vec2_t size;
    uint32_t channel_count;
    unsigned char* data;
    bool filter, sRGB;
} texture_t;

void texture_create_from_png(texture_t *texture, const char *path, bool filter, bool sRGB);
void texture_create_from_data(texture_t *texture, unsigned char *data, vec2_t size, uint32_t channels, bool filter, bool sRGB);
void texture_bind(texture_t *texture, uint32_t index);
void texture_unbind(uint32_t index);
void texture_delete(texture_t *texture);
void texture_update_data(texture_t *texture);
