#include "api.h"

window_t window =
{
    .width = 800,
    .height = 600,
    .vsync = true,
    .title = "okapi",
    .cursor = false
};

renderer_t renderer =
{
    .window = &window,
    .clear_color = {.9, .9,1, 1},
    .width = 800 * 2 ,
    .height = 600 * 2
};

typedef struct
{
    f32 fov, near, far;

    bool orthographic;
    f32 orthographic_size;

    vec3_t positition;
    vec4_t rotation;
} camera_t;

camera_t camera =
{
    .fov = 30,
    .near = 0.1,
    .far = 1000,
    .orthographic = false,
    .orthographic_size = 1,
    .positition = {0,4,20}
};

typedef struct
{
    vec3_t position;
    vec4_t rotation;
    f32 scale;
}player_t;

player_t player =
{
    .position = {0,0,0},
    .scale = .01f,
    .rotation = (vec4_t){0,0,0,1}
};

typedef struct
{
    vec3_t direction;
    float strenght;
    vec4_t color;
}directional_light_t;


directional_light_t sun =
{
    .direction = {0,-1,-1},
    .strenght = 1,
    .color = {1,1,1,1}
};


model_3D_t landscape_model = {};
model_3D_t player_model = {};

void render()
{
    renderer_draw_model_3D(&renderer, &player_model, player.position, player.scale, player.rotation);
    renderer_draw_model_3D(&renderer, &landscape_model, (vec3_t) { 0, 0, 1 }, 1, (vec4_t) { 0, 0, 0, 1 });
}

int main()
{
    window_create(&window);
    renderer_create(&renderer);

    model_3D_load_from_bin(&landscape_model, "./assets/meshes/forest.okp3d");
    model_3D_create(&landscape_model);


    model_3D_load_from_bin(&player_model, "./assets/meshes/nude.okp3d");
    model_3D_create(&player_model);

    mesh_animation_t* dance_animation = mesh_anim_from_name(&player_model, "running");

    while (!window.closed)
    {
        window_update(&window);

#include "thirdperson.h"
        renderer.proj_mat = mat4_perspective(camera.fov, window.aspect, camera.near, camera.far);
        renderer.view_mat = mat4_look_at(camera.positition, vec3_add(player.position, (vec3_t) { 0, 1, 0 }), (vec3_t) { 0, 1, 0 });

        //-------------------------------------------------//
        animation_play(&player_model.armature, dance_animation, window.dt, 1, true);
        //-------------------------------------------------//

        renderer_start(&renderer);
        shader_set_uniform_vec3(&renderer.mesh_shader, "u_light_dir", sun.direction);

        render();

        renderer_end(&renderer);
    }
    model_3D_delete(&player_model);
    model_3D_delete(&landscape_model);

    renderer_exit(&renderer);
    window_exit(&window);
}
