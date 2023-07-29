#include "api.h"

window_t window;
renderer_t renderer;

#include "loop.h"

int main(void)
{
    init();

    window.size.x = WINDOW_SIZE;
    window.size.y = WINDOW_SIZE;
    window.title = "Hello";
    window.vsync = 1;


    window_init(&window);
    renderer_create(&renderer, (vec4_t){1, 1, 1, 1}, (vec2_t){WINDOW_SIZE, WINDOW_SIZE});

#define IMAGE_SIZE 256
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
    texture_create_from_data(&tex1, texture_data, (vec2_t){IMAGE_SIZE, IMAGE_SIZE}, true);

    model_3D_t woman;
    model_3D_create_from_file(&woman, "./res/3D/woman/woman.gltf");

    model_3D_t scene;
    model_3D_create_from_file(&scene, "./res/3D/scene/scene.gltf");

    while (!window.closed)
    {
        window_update(&window);
        renderer_start(&renderer);

        update();

        if (!key_pressed(&window, KEY_SPACE))
            renderer.proj_mat = mat4_perspective(fov, window.aspect, 0.1f, 100.0f);
        else
            renderer.proj_mat = mat4_ortho_aspect(window.aspect, 1, -20, 20);

        renderer.view_mat = mat4_look_at(cam_pos, vec3_add(cam_pos, cam_front), cam_up);

        renderer_draw_model_3D(&renderer, &woman, (vec3_t){0, 0, -1}, 1, (vec3_t){});
        renderer_draw_model_3D(&renderer, &scene, (vec3_t){2, 0, -1}, .5, (vec3_t){});

        /* renderer_batch_start(&renderer);
        renderer_draw_quad(&renderer, &tex1, (vec3_t){0.0f, 0.0f, -3.0f}, (vec3_t){2, 2}, (vec4_t){1.0f, 1.0f, 1.0f, 1.0f});
        renderer_batch_end(&renderer); */

        renderer_end(&renderer);
    }

    model_3D_delete(&woman);

    renderer_exit(&renderer);
    window_exit(&window);
}
