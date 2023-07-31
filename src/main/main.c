#include "api.h"

window_t window;
renderer_t renderer;

#include "loop.h"

int main(void)
{
    init();

    window_create(&window, (vec2_t){800, 600}, "OKAPI", 0);
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
            texture_data[(x * 4) + (y * IMAGE_SIZE * 4) + 3] = rand() % 256;
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

    while (!window.closed)
    {
        window_update(&window);
        update();

        renderer.view_mat = mat4_look_at((cam_pos), vec3_add(cam_pos, cam_front), cam_up);

        renderer_start(&renderer, &camera);

        renderer_draw_model_3D(&renderer, &scene, (vec3_t){}, .5, (vec3_t){});
        renderer_draw_model_3D(&renderer, &clone_trooper, (vec3_t){0, 0.1, 2.5}, .2, (vec3_t){0, glfwGetTime() * 100, 0});

        renderer_batch_start(&renderer);
        renderer_draw_quad(&renderer, &tex1, (vec3_t){0.0f, 2.5f, -3.0f}, (vec3_t){5, 5}, (vec4_t){1.0f, 1.0f, 1.0f, 1.0f});
        renderer_batch_end(&renderer);

        renderer_end(&renderer);

        if (window.resized)
        {
            printf("resized\n");
        }
    }

    model_3D_delete(&scene);

    renderer_exit(&renderer);
    window_exit(&window);
}
