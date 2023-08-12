#include "api.h"

int main()
{
    window_t window =
    {
        .width = 800 * 2,
        .height = 600 * 2,
        .vsync = true,
        .title = "okapi"
    };
    window_create(&window);

    renderer_t renderer =
    {
        .window = &window,
        .clear_color = {.9, .9, 1, .5},
        .width = 800 * 2,
        .height = 800 * 2 / window.aspect,
        .msaa = false
    };
    renderer_create(&renderer);

    camera_t camera =
    {
        .fov = 30,
        .near = 0.1,
        .far = 100,
        .orthographic = false,
        .orthographic_size = 1
    };

    model_3D_t player_model = {};
    model_3D_load_from_GLTF(&player_model, "./res/3D/knight/knight.gltf");

    while (!window.closed)
    {
        window_update(&window);
        if (camera.orthographic)
            renderer.proj_mat = mat4_ortho_aspect(window.aspect, camera.orthographic_size, camera.near, camera.far);
        else
            renderer.proj_mat = mat4_perspective(camera.fov, window.aspect, camera.near, camera.far);

        //-------------------------------------------------//
        
        static vec3_t cam_pos = { 0,1,6 };
        float cam_speed = 3;
        if (key_pressed(&window, KEY_LEFT_SHIFT))
            cam_speed = 5;

        if (key_pressed(&window, KEY_W))
            cam_pos.z -= cam_speed * window.dt;
        else if (key_pressed(&window, KEY_S))
            cam_pos.z += cam_speed * window.dt;
        if (key_pressed(&window, KEY_A))
            cam_pos.x -= cam_speed * window.dt;
        else if (key_pressed(&window, KEY_D))
            cam_pos.x += cam_speed * window.dt;
        if (key_pressed(&window, KEY_LEFT_ALT))
            cam_pos.y -= cam_speed * window.dt;
        else if (key_pressed(&window, KEY_SPACE))
            cam_pos.y += cam_speed * window.dt;

        renderer.view_mat = mat4_look_at(cam_pos, vec3_add(cam_pos, (vec3_t) { 0, 0, -1 }), (vec3_t) { 0, 1, 0 });

        //-------------------------------------------------//

        renderer_start(&renderer);

        static float rot = 0;
        if (key_pressed(&window, KEY_R))
            rot += window.dt * 80;

        renderer_draw_model_3D(&renderer, &camera, &player_model, (vec3_t) { 0, 0, 1 }, 1, quat_angle_axis(rot, (vec3_t) { 0, 1, 0 }));
       
        renderer_end(&renderer);
    }
    model_3D_delete(&player_model);


    renderer_exit(&renderer);
    window_exit(&window);
}
