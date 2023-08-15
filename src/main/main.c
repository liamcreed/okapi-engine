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
        .clear_color = {.7, .9, 1, .5},
        .width = 800  ,
        .height = 800 / window.aspect,
        .msaa = true
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


    model_3D_t landscape = {};
    model_3D_load_from_GLTF(&landscape, "./res/3D/landscape/landscape.gltf");

    model_3D_create(&landscape);

    typedef struct
    {
        vec3_t position;
        vec4_t rotation;
    }player_t;

    player_t player =
    {
        .position = {0,0,0},
        .rotation = quat_angle_axis(180, (vec3_t) { 0,1,0 })
    };

    model_3D_t player_model = {};
    model_3D_load_from_bin(&player_model, "./res/model.3D");
    model_3D_create(&player_model);
    
        mesh_animation_t* walk_animation = mesh_anim_from_name(&player_model, "walking");
        mesh_animation_t* idle_animation = mesh_anim_from_name(&player_model, "dance");
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

        renderer.view_mat = mat4_look_at(camera.positition, vec3_add(player.position, (vec3_t) { 0, 1, 0 }), (vec3_t) { 0, 1, 0 });

        //-------------------------------------------------//

        renderer_start(&renderer);

        renderer_draw_model_3D(&renderer, &camera, &player_model, player.position, 1, player.rotation);

        renderer_draw_model_3D(&renderer, &camera, &landscape, (vec3_t) { 0, 0, 1 }, .1, (vec4_t) { 0, 0, 0, 1 });

        renderer_end(&renderer);
    }
    model_3D_delete(&player_model);


    renderer_exit(&renderer);
    window_exit(&window);
}
