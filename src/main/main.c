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
        .clear_color = (vec4_t){.7, .9, 1, .5},
        .width = 200,
        .height = 150
    };

    renderer_create(&renderer);

    camera_t camera =
    {
        .fov = 45,
        .near = 0.1,
        .far = 100
    };

    camera_create_perspective(&camera);

    model_3D_t player_model = {};
    model_3D_load_from_GLTF(&player_model, "./res/3D/knight/knight.gltf");




    texture_t texture = {};
    texture.sRGB = true;
    texture_load_from_TGA(&texture, "./res/textures/sprite.tga");
    texture_create(&texture);

    while (!window.closed)
    {
        window_update(&window);
        if (camera.orthographic)
            renderer.proj_mat = mat4_ortho_aspect(window.aspect, camera.orthographic_size, camera.near, camera.far);
        else
            renderer.proj_mat = mat4_perspective(camera.fov, window.aspect, camera.near, camera.far);

        static vec3_t cam_pos = (vec3_t){0,0,4};
        float cam_speed = 3;
        if(key_pressed(&window, KEY_LEFT_SHIFT))
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

        renderer.view_mat = mat4_look_at(cam_pos, (vec3_t) { 0, 0, 0 }, (vec3_t) { 0, 1, 0 });

        //-------------------------------------------------//

        renderer_start(&renderer);

        renderer_draw_model_3D(&renderer, &camera, &player_model, (vec3_t) { 0, 0, 1 }, 1., quat_angle_axis(window.mouse_pos.x / 2, (vec3_t) { 0, 1, 0 }));
        renderer_draw_quad(&renderer,  &texture, (vec4_t) { 1, 1, 1, 1 }, (vec3_t) { 0, 0, 0 }, (vec3_t) { 5, 5, 1 }, (vec4_t) {});

        renderer_end(&renderer);
    }

    renderer_exit(&renderer);
    window_exit(&window);
}
 