#include "graphics/graphics.h"

void texture_load_from_TGA(texture_t* texture, const char* path)
{
    FILE* file;
    file = fopen(path, "rb");
    if (file == NULL)
        printf(LOG_ERROR"[Texture]: failed to open %s\n", path);

    tga_header_t tga_header = {};
    fread(&tga_header, sizeof(tga_header_t), 1, file);

    texture->width = tga_header.width;
    texture->height = tga_header.height;

    texture->channel_count = tga_header.bits_per_pixel / 8;

    u32 length = texture->width * texture->height * texture->channel_count;
    u8* image_data = malloc(length);
    texture->data = malloc(length);
    fread(image_data, length, 1, file);
    if (image_data == NULL)
        printf(LOG_ERROR"[Texture]: failed to get data from %s\n", path);


    for (int y = 0; y < texture->height; y++)
    {
        for (int x = 0; x < texture->width; x++)
        {
            int original_index = (y * texture->width + x) * texture->channel_count;
            int flipped_index = ((texture->height - y - 1) * texture->width + x) * texture->channel_count;

            texture->data[original_index + 0] = image_data[flipped_index + 2];
            texture->data[original_index + 1] = image_data[flipped_index + 1];
            texture->data[original_index + 2] = image_data[flipped_index + 0];
            if (texture->channel_count == 4)
                texture->data[original_index + 3] = image_data[flipped_index + 3];
        }
    }

    free(image_data);

    fclose(file);
}



void texture_load_from_bin(texture_t* texture, const char* path)
{
    FILE* file;
    file = fopen(path, "rb");
    if (file == NULL)
    {
        printf(LOG_ERROR "[texture]: %s does not exist!\n", path);
    }
    fread(&texture->channel_count, sizeof(u32), 1, file);
    fread(&texture->width, sizeof(f32), 1, file);
    fread(&texture->height, sizeof(f32), 1, file);
    texture->data = malloc(texture->width * texture->height * texture->channel_count);
    fread(texture->data, texture->width * texture->height * texture->channel_count, 1, file);

    fclose(file);
}


void texture_TGA_to_bin(const char* TGA, const char* bin)
{
    texture_t texture = {};
    texture_load_from_TGA(&texture, TGA);
    texture_export_to_bin(&texture, bin);
}

void texture_export_to_bin(texture_t* texture, const char* path)
{
    FILE* file;
    file = fopen(path, "wb");
    fwrite(&texture->channel_count, sizeof(u32), 1, file);
    fwrite(&texture->width, sizeof(f32), 1, file);
    fwrite(&texture->height, sizeof(f32), 1, file);
    fwrite(texture->data, texture->width * texture->height * texture->channel_count, 1, file);
    fclose(file);
}

void texture_create(texture_t* texture)
{
    if (texture->data == NULL)
        printf(LOG_WARNING "[texture]: data is empty!\n");
    if (texture->channel_count == 0)
        printf(LOG_WARNING "[texture]: texure doesn't contain channel count!\n");
    if (texture->width == 0 || texture->height == 0)
        printf(LOG_WARNING "[texture]: texure doesn't have a size!\n");

    GL(glGenTextures(1, &texture->id));
    GL(glBindTexture(GL_TEXTURE_2D, texture->id));

    GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

    if (texture->filter)
    {
        GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    }
    else
    {
        GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    }


    if (texture->channel_count == 4 && texture->sRGB == true)
    {
        GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->data));
    }
    else if (texture->channel_count == 3 && texture->sRGB == true)
    {
        GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, texture->width, texture->height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture->data));
    }
    else if (texture->channel_count == 4 && texture->sRGB == false)
    {
        GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->data));
    }
    else if (texture->channel_count == 3 && texture->sRGB == false)
    {
        GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture->width, texture->height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture->data));
    }

    GL(glGenerateMipmap(GL_TEXTURE_2D));
    GL(glBindTexture(GL_TEXTURE_2D, 0));

}

void texture_update_data(texture_t* texture)
{
    GLenum format;
    if (texture->channel_count == 4)
        format = GL_RGBA;
    else if (texture->channel_count == 3)
        format = GL_RGB;

    GL(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texture->width, texture->height, format, GL_UNSIGNED_BYTE, texture->data));
}

void texture_bind(texture_t* texture, u32 index)
{
    GL(glActiveTexture(GL_TEXTURE0 + index));
    GL(glBindTexture(GL_TEXTURE_2D, texture->id));
}
void texture_unbind(u32 index)
{
    GL(glActiveTexture(GL_TEXTURE0 + index));
    GL(glBindTexture(GL_TEXTURE_2D, 0));
}

void texture_delete(texture_t* texture)
{
    // free(texture->data);
    GL(glDeleteTextures(1, &texture->id));
}
