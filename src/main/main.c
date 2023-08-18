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
        .clear_color = {.9, .9,1, 1},
        .width = 800 * 2,
        .height = 800 * 2 / window.aspect
    };
    renderer_create(&renderer);

    camera_t camera =
    {
        .fov = 30,
        .near = 0.1,
        .far = 1000,
        .orthographic = false,
        .orthographic_size = 1,
        .positition = {0,4,20}
    };

    model_3D_t landscape = {};
    model_3D_load_from_bin(&landscape, "./assets/meshes/landscape.okp3d");

    model_3D_create(&landscape);

    typedef struct
    {
        vec3_t position;
        vec4_t rotation;
        f32 scale;
    }player_t;

    player_t player =
    {
        .position = {0,0,0},
        .scale = .01,
        .rotation = quat_angle_axis(180, (vec3_t) { 0,1,0 })
    };

    typedef struct
    {
        vec3_t direction;
        float strenght;
        vec4_t color;
    }directional_light_t;

    directional_light_t sun =
    {
        .direction = {-1,-1,-1},
        .strenght = 1,
        .color = {1,1,1,1}
    };

    model_3D_t player_model = {};
    model_3D_load_from_bin(&player_model, "./assets/meshes/character.okp3d");
    model_3D_create(&player_model);

    mesh_animation_t* walk_animation = mesh_anim_from_name(&player_model, "running");
    mesh_animation_t* idle_animation = mesh_anim_from_name(&player_model, "running");
    mesh_animation_t* run_animation = mesh_anim_from_name(&player_model, "running");

    while (!window.closed)
    {
        window_update(&window);

        if (camera.orthographic)
            renderer.proj_mat = mat4_ortho_aspect(window.aspect, camera.orthographic_size, camera.near, camera.far);
        else
            renderer.proj_mat = mat4_perspective(camera.fov, window.aspect, camera.near, camera.far);

        //-------------------------------------------------//
#include "thirdperson.h"

        if (key_being_pressed(&window, KEY_LEFT_SHIFT))
            animation_play(&player_model.armature, run_animation, window.dt, 1, true);

        renderer.view_mat = mat4_look_at(camera.positition, vec3_add(player.position, (vec3_t) { 0, 1, 0 }), (vec3_t) { 0, 1, 0 });

        //-------------------------------------------------//

        renderer_start(&renderer);

        shader_set_uniform_vec3(&renderer.mesh_shader, "u_light_dir", sun.direction);

        renderer_draw_model_3D(&renderer, &player_model, player.position, player.scale, player.rotation);
        renderer_draw_model_3D(&renderer, &landscape, (vec3_t) { 0, 0, 1 }, 1, (vec4_t) { 0, 0, 0, 1 });

        renderer_end(&renderer);
    }
    model_3D_delete(&player_model);


    renderer_exit(&renderer);
    window_exit(&window);
}
