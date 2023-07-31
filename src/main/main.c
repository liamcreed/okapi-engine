#include "api.h"

window_t window;
renderer_t renderer;

#include "loop.h"

#include "stb_image/stb_image.h"

int main(void)
{
    init();

    window_create(&window, (vec2_t){800 * 2, 600 * 2}, "OKAPI", 1);
    renderer_create(&renderer, &window, (vec4_t){.7, .9, 1, .5}, 1);

#define IMAGE_SIZE 100
    unsigned char texture_data[IMAGE_SIZE * IMAGE_SIZE * 4];
    for (int x = 0; x < IMAGE_SIZE; x++)
    {
        for (int y = 0; y < IMAGE_SIZE; y++)
        {
            texture_data[(x * 4) + (y * IMAGE_SIZE * 4) + 0] = (float)x / IMAGE_SIZE * 255;
            texture_data[(x * 4) + (y * IMAGE_SIZE * 4) + 1] = (float)y / IMAGE_SIZE * 255;
            texture_data[(x * 4) + (y * IMAGE_SIZE * 4) + 2] = 255;
            texture_data[(x * 4) + (y * IMAGE_SIZE * 4) + 3] = 255;
        }
    }
    texture_t tex1;
    texture_create_from_data(&tex1, texture_data, (vec2_t){IMAGE_SIZE, IMAGE_SIZE}, false);

    model_3D_t scene;
    model_3D_create_from_file(&scene, "./res/3D/landscape/landscape.gltf");

    model_3D_t clone_trooper;
    model_3D_create_from_file(&clone_trooper, "./res/3D/clone/clone.gltf");

    camera_t camera;
    camera_create_perspective(&camera, 45, 0.1, 100);

    const char *cubemap_faces[6] = {
        "./res/textures/skybox/right.jpg",
        "./res/textures/skybox/left.jpg",
        "./res/textures/skybox/top.jpg",
        "./res/textures/skybox/bottom.jpg",
        "./res/textures/skybox/front.jpg",
        "./res/textures/skybox/back.jpg"};
    int width, height, channel_count;
    for (unsigned int i = 0; i < 6; i++)
    {
        unsigned char *data = stbi_load(cubemap_faces[i], &width, &height, &channel_count, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
            printf(LOG_INFO "Cubemap tex loaded at path: %s\n", cubemap_faces[i]);
        }
        else
        {
            printf(LOG_ERROR "Cubemap tex failed to load at path: %s\n", cubemap_faces[i]);
            stbi_image_free(data);
        }
    }

    uint32_t cubemap_tex;
    glGenTextures(1, &cubemap_tex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_tex);

    float res_scale = .7;
    framebuffer_t framebuffer;
    framebuffer_create(&framebuffer, (vec2_t){window.size.x * res_scale, window.size.y * res_scale}, false);

    float fbo_vertices[] = {
        -1.0f, 1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 0.0f,

        -1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f};

    vertex_array_t fbo_va;
    vertex_array_create(&fbo_va);
    vertex_array_create_vbo(&fbo_va, fbo_vertices, sizeof(fbo_vertices), false);
    vertex_array_push_attribute(0, 2, 4 * sizeof(float), 0);
    vertex_array_push_attribute(1, 2, 4 * sizeof(float), 2 * sizeof(float));

    shader_t pp_shader;
    shader_create(&pp_shader, "./res/shaders/pp.vert", "./res/shaders/pp.frag");

    while (!window.closed)
    {
        window_update(&window);
        update();

        renderer.view_mat = mat4_look_at((cam_pos), vec3_add(cam_pos, cam_front), cam_up);
        if (key_pressed(&window, KEY_SPACE))
            camera.orthographic = true;
        else
            camera.orthographic = false;

        framebuffer_bind(&framebuffer);
        renderer_start(&renderer, &camera);

        renderer_draw_model_3D(&renderer, &scene, (vec3_t){}, .5, (vec3_t){});
        renderer_draw_model_3D(&renderer, &clone_trooper, (vec3_t){0, 0.1, 2.5}, .2, (vec3_t){0, glfwGetTime() * 100, 0});

        renderer_batch_start(&renderer);
        renderer_draw_quad(&renderer, &tex1, (vec3_t){-10.0f, 2.5f, -3.0f}, (vec3_t){5, 5}, (vec4_t){1.0f, 1.0f, 1.0f, 1.0f});
        renderer_draw_quad(&renderer, &tex1, (vec3_t){-5.0f, 2.5f, -3.0f}, (vec3_t){5, 5}, (vec4_t){1.0f, 1.0f, 1.0f, 1.0f});
        renderer_draw_quad(&renderer, &tex1, (vec3_t){0.0f, 2.5f, -3.0f}, (vec3_t){5, 5}, (vec4_t){1.0f, 1.0f, 1.0f, 1.0f});
        renderer_draw_quad(&renderer, &tex1, (vec3_t){5.0f, 2.5f, -3.0f}, (vec3_t){5, 5}, (vec4_t){1.0f, 1.0f, 1.0f, 1.0f});
        renderer_draw_quad(&renderer, &tex1, (vec3_t){10.0f, 2.5f, -3.0f}, (vec3_t){5, 5}, (vec4_t){1.0f, 1.0f, 1.0f, 1.0f});
        renderer_batch_end(&renderer);

        renderer_end(&renderer);
        framebuffer_unbind();
        glDisable(GL_DEPTH_TEST);

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glViewport(0, 0, window.size.x, window.size.y);

        shader_bind(&pp_shader);
        vertex_array_bind(&fbo_va);
        texture_bind(&framebuffer.texture, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    model_3D_delete(&scene);

    renderer_exit(&renderer);
    window_exit(&window);
}
