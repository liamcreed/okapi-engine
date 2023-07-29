#include "graphics/graphics.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

void texture_create_from_file(texture_t *texture, const char *path, bool filter)
{
    unsigned char *data;
    stbi_set_flip_vertically_on_load(1);
    int channel_count;
    int x, y;
    data = stbi_load(path, &x, &y, &channel_count, 0);

    texture->size.x = x;
    texture->size.y = y;
    texture->channel_count = channel_count;

    if (data == NULL)
    {
        printf(LOG_ERROR "[texture]: failed to load texture: %s\n", path);
        exit(-1);
    }
    else
    {
        printf(LOG_INFO "[texture]: Succesfully loaded texture: |%s|\n", path);
    }

    glGenTextures(1, &texture->id);
    glBindTexture(GL_TEXTURE_2D, texture->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
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

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->size.x, texture->size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    free(data);
}

void texture_create_from_data(texture_t *texture, unsigned char *data, vec2_t size, bool filter)
{
    if (data == NULL)
    {
        printf(LOG_WARNING "[texture]: data is empty!\n");
    }

    texture->size = size;
    texture->channel_count = 4;

    glGenTextures(1, &texture->id);
    glBindTexture(GL_TEXTURE_2D, texture->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
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

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->size.x, texture->size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void texture_update_data(texture_t *texture, unsigned char *data, vec2_t size)
{
    texture->size = size;
    texture->channel_count = 4;
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texture->size.x, texture->size.y, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

void texture_bind(texture_t *texture, uint32_t index)
{
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, texture->id);
}
void texture_unbind(uint32_t index)
{
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void texture_delete(texture_t *texture)
{
    glDeleteTextures(1, &texture->id);
}

sub_texture_t sub_texture_create(vec2_t tex_size, vec2_t coords, vec2_t sub_tex_size)
{
    sub_texture_t sub_texture;
    sub_texture.sub_tex_size = sub_tex_size;
    sub_texture.tex_size = tex_size;

    vec2_t min = {(coords.x * sub_texture.sub_tex_size.x) / sub_texture.tex_size.x, (coords.y * sub_texture.sub_tex_size.y) / sub_texture.tex_size.y};
    vec2_t max = {((coords.x + 1) * sub_texture.sub_tex_size.x) / sub_texture.tex_size.x, ((coords.y + 1) * sub_texture.sub_tex_size.y) / sub_texture.tex_size.y};

    sub_texture.uv[0] = (vec2_t){min.x, min.y};
    sub_texture.uv[1] = (vec2_t){max.x, min.y};
    sub_texture.uv[2] = (vec2_t){max.x, max.y};
    sub_texture.uv[3] = (vec2_t){min.x, max.y};
    return sub_texture;
}