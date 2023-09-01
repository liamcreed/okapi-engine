#include "api.h"

window_t window =
{
    .width = 800 * 0.7,
    .height = 600 * 0.7,
    .vsync = false,
    .title = "okapi",
    .cursor = false
};

renderer_t renderer =
{
    .window = &window,
    .clear_color = {.9, .9,1, 1},
    .width = 200 * 2.1 ,
    .height = 200 * 2.1 * (800.0/600.0)
};

typedef struct
{
    f32 fov, near, far;

    bool orthographic;
    f32 orthographic_size;

    vec3 positition;
    vec4 rotation;
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
    vec3 position;
    vec4 rotation;
    f32 scale;
}player_t;

player_t player =
{
    .position = {0,0,0},
    .scale = .01f,
    .rotation = (vec4){0,0,0,1}
};

typedef struct
{
    vec3 direction;
    float strenght;
    vec4 color;
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
    renderer_draw_model_3D(&renderer, &landscape_model, (vec3) { 0, 0, 1 }, 1, (vec4) { 0, 0, 0, 1 });
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

#include "camera.h"
        renderer.proj_mat = mat4_perspective(camera.fov, window.aspect, camera.near, camera.far);
        renderer.view_mat = mat4_look_at(camera.positition, vec3_add(player.position, (vec3) { 0, 3, 3 }), (vec3) { 0, 1, 0 });

        //-------------------------------------------------//
        if(x_input || z_input)
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
