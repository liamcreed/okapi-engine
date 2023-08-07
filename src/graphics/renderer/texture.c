#include "graphics/graphics.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

void texture_create_from_png(texture_t* texture, const char* path)
{
    stbi_set_flip_vertically_on_load(true);
    i32 channel_count;
    i32 x, y;
    texture->data = stbi_load(path, &x, &y, &channel_count, 0);
    texture->size.x = x;
    texture->size.y = y;
    texture->channel_count = channel_count;

    if (texture->data == NULL)
    {
        printf(LOG_ERROR "[texture]: failed to load texture: %s\n", path);
        exit(-1);
    }

    texture_create_from_data(texture);
}

void texture_create_from_bin(texture_t* texture, const char* path)
{
    FILE* file;
    file = fopen(path, "rb");
    if (file == NULL)
    {
        printf(LOG_ERROR "[texture]: %s does not exist!\n", path);
    }
    fread(&texture->channel_count, sizeof(u32), 1, file);
    fread(&texture->size.x, sizeof(f32), 1, file);
    fread(&texture->size.y, sizeof(f32), 1, file);
    texture->data = malloc(texture->size.x * texture->size.y * texture->channel_count);
    fread(texture->data, texture->size.x * texture->size.y * texture->channel_count, 1, file);

    texture_create_from_data(texture);
    fclose(file);
}

void texture_export_to_bin(texture_t* texture, const char* path)
{
    FILE *file;
    file = fopen(path, "wb");
    fwrite(&texture->channel_count, sizeof(u32), 1, file);
    fwrite(&texture->size.x, sizeof(f32), 1, file);
    fwrite(&texture->size.y, sizeof(f32), 1, file);
    fwrite(texture->data, texture->size.x * texture->size.y * texture->channel_count, 1, file);
    fclose(file);
}

void texture_create_from_data(texture_t* texture)
{
    if (texture->data == NULL)
        printf(LOG_WARNING "[texture]: data is empty!\n");
    if (texture->channel_count == 0)
        printf(LOG_WARNING "[texture]: texure doesn't contain channel count!\n");
    if (texture->size.x == 0 || texture->size.y == 0)
        printf(LOG_WARNING "[texture]: texure doesn't have a size!\n");

    glGenTextures(1, &texture->id);
    glBindTexture(GL_TEXTURE_2D, texture->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    if (texture->filter)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    if (texture->channel_count == 4 && texture->sRGB == true)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, texture->size.x, texture->size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->data);
    else if (texture->channel_count == 3 && texture->sRGB == true)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, texture->size.x, texture->size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, texture->data);
    else if (texture->channel_count == 4 && texture->sRGB == false)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->size.x, texture->size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->data);
    else if (texture->channel_count == 3 && texture->sRGB == false)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture->size.x, texture->size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, texture->data);

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void texture_create_framebuffer_tex(texture_t* texture, vec2_t size, bool filter)
{
    glGenTextures(1, &texture->id);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    if (filter)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->id, 0);
}

void texture_update_data(texture_t* texture)
{
    if (texture->channel_count == 4)
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texture->size.x, texture->size.y, GL_RGBA, GL_UNSIGNED_BYTE, texture->data);
    else if (texture->channel_count == 3)
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texture->size.x, texture->size.y, GL_RGB, GL_UNSIGNED_BYTE, texture->data);
}

void texture_bind(texture_t* texture, u32 index)
{
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, texture->id);
}
void texture_unbind(u32 index)
{
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void texture_delete(texture_t* texture)
{
    free(texture->data);
    glDeleteTextures(1, &texture->id);
}
