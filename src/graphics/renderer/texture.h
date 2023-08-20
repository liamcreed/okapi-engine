#pragma once

#pragma pack(push, 1)
typedef struct
{
    u8  id_length;
    u8  colourmap_type;
    u8  datatype_code;
    u16 colourmap_origin;
    u16 colourmap_length;
    u8  colourmap_depth;
    u16 x_origin;
    u16 y_origin;
    u16 width;
    u16 height;
    u8  bits_per_pixel;
    u8  image_descriptor;
}tga_header_t;
#pragma pack(pop)

typedef struct
{
    u32 id;
    bool filter, sRGB;

    u32 width;
    u32 height;
    u32 channel_count;
    u8* data;
} texture_t;

void texture_load_from_TGA(texture_t *texture, const char *path);

void texture_export_to_bin(texture_t* texture, const char* path);
void texture_TGA_to_bin(const char* TGA, const char* bin);
void texture_load_from_bin(texture_t *texture, const char *path);
void texture_create(texture_t *texture);
void texture_bind(texture_t *texture, u32 index);
void texture_unbind(u32 index);
void texture_delete(texture_t *texture);
void texture_update_data(texture_t *texture);
