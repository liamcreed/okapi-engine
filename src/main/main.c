#include "api.h"

window_t window;
renderer_t renderer;

camera_t camera;
#include "loop.h"

#include "stb_image/stb_image.h"

int main(void)
{
    init();

    window_create(&window, (vec2_t){800 * 2, 600 * 2}, "OKAPI", 1);
    renderer_create(&renderer, &window, (vec4_t){.7, .9, 1, .5}, 1);

    model_3D_t scene;
    model_3D_create_from_file(&scene, "./res/3D/landscape/landscape.gltf");

    model_3D_t clone_trooper;
    model_3D_create_from_file(&clone_trooper, "./res/3D/monkey/monkey.gltf");

    camera_create_perspective(&camera, 45, 0.1, 100);
    camera.pos = (vec3_t){0, .5, 4};

    const char *cubemap_faces[6] = {
        "./res/textures/skybox/right.jpg",
        "./res/textures/skybox/left.jpg",
        "./res/textures/skybox/top.jpg",
        "./res/textures/skybox/bottom.jpg",
        "./res/textures/skybox/front.jpg",
        "./res/textures/skybox/back.jpg"};

    skybox_t skybox;
    skybox_load_from_files(&skybox, cubemap_faces, false);

    while (!window.closed)
    {
        window_update(&window);
        update();

        if (key_pressed(&window, KEY_SPACE))
            camera.orthographic = true;
        else
            camera.orthographic = false;

        renderer_start(&renderer, &camera);

        renderer_draw_model_3D(&renderer, &camera, &scene, (vec3_t){}, .5, (vec3_t){});
        renderer_draw_model_3D(&renderer, &camera, &clone_trooper, (vec3_t){0, 0.5, 2.5}, .2, (vec3_t){0, glfwGetTime() * 1, 0});

        renderer_draw_skybox(&renderer, &skybox);

        renderer_end(&renderer);
    }

    renderer_exit(&renderer);
    window_exit(&window);
}
