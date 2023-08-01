#include "graphics/graphics.h"
#include "stb_image/stb_image.h"

float skybox_vertices[] = {
    // positions
    -1.0f, 1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,

    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,

    -1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 1.0f};

void cubemap_load_from_files(cubemap_t *cubemap, const char **files, uint32_t count, bool filter)
{
    glGenTextures(1, &cubemap->id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap->id);

    stbi_set_flip_vertically_on_load(false);
    int channel_count;
    int x, y;
    for (unsigned int i = 0; i < count; i++)
    {
        unsigned char *data = stbi_load(files[i], &x, &y, &channel_count, 0);
        cubemap->size.x = x;
        cubemap->size.y = y;
        cubemap->channel_count = channel_count;
        if (data)
        {
            if (cubemap->channel_count == 3)
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                             0, GL_RGB, cubemap->size.x, cubemap->size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            else
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                             0, GL_RGB, cubemap->size.x, cubemap->size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
            printf(LOG_INFO "Cubemap tex loaded at path: %s\n", files[i]);
        }
        else
        {
            printf(LOG_ERROR "Cubemap tex failed to load at path: %s\n", files[i]);
            stbi_image_free(data);
        }
    }
    
    if (filter)
    {
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void skybox_load_from_files(skybox_t *skybox, const char **files, bool filter)
{
    skybox->vertices = skybox_vertices;
    if(skybox->vertices == NULL)
        printf("asssss\n");
    
    cubemap_load_from_files(&skybox->cubemap, files, 6, filter);

    vertex_array_create(&skybox->va);
    vertex_array_create_vbo(&skybox->va, skybox->vertices, sizeof(skybox_vertices), false);
    vertex_array_push_attribute(0, 3, 3 * sizeof(float), 0);
    vertex_array_unbind();
    vertex_array_unbind_buffers(); 
}
