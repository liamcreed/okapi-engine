#include "api.h"

int main(void)
{
    mat4_inverse(mat4_new(1));
    window_t window;
    window.size.x = 800 * 2;
    window.size.y = 600 * 2;
    window.vsync = true;
    window.title = "okapi";
    
    window_create(&window);

    renderer_t renderer;
    renderer.window = &window;
    renderer.clear_color = (vec4_t){.7, .9, 1, .5};
    renderer.resolution = (vec2_t){200, 200/ window.aspect};
    renderer_create(&renderer);

    camera_t camera;
    camera_create_perspective(&camera, 45, 0.1, 100);

    model_3D_t player_model;
    model_3D_create_from_file(&player_model, "./res/3D/knight/knight.gltf");
    

    /* const char *cubemap_faces[] = {
        "./res/textures/skybox/right.jpg",
        "./res/textures/skybox/left.jpg",
        "./res/textures/skybox/top.jpg",
        "./res/textures/skybox/bottom.jpg",
        "./res/textures/skybox/front.jpg",
        "./res/textures/skybox/back.jpg"};

    skybox_t skybox;
    skybox_create_from_files(&skybox, cubemap_faces, false);  */

    texture_t tex;
    texture_create_from_png(&tex, "./res/textures/tilemap.png", false, true);

    float vertices[] = {
        // positions         // texture coords
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f   // top lef
    };
    uint32_t indices[] = {
        0, 1, 3,
        1, 2, 3};

    vertex_array_t quad_va;
    vertex_array_create(&quad_va);
    vertex_array_create_vbo(&quad_va, vertices, sizeof(vertices), false);
    vertex_array_push_attribute_f(0, 3, 5 * sizeof(float), 0);
    vertex_array_push_attribute_f(2, 2, 5 * sizeof(float), 3 * sizeof(float));
    vertex_array_create_ibo(&quad_va, indices, sizeof(indices), false);

    while (!window.closed)
    {
        window_update(&window);
        if (camera.orthographic)
            renderer.proj_mat = mat4_ortho_aspect(window.aspect, camera.orthographic_size, camera.near, camera.far);
        else
            renderer.proj_mat = mat4_perspective(camera.fov, window.aspect, camera.near, camera.far);

        static float cam_z = 2;
        if (key_pressed(&window, KEY_W))
            cam_z -= 1 * window.dt;
        else if (key_pressed(&window, KEY_S))
            cam_z += 1 * window.dt;

        renderer.view_mat = mat4_look_at((vec3_t){0, 0, cam_z}, (vec3_t){0, 0, 0}, (vec3_t){0, 1, 0});

        //-------------------------------------------------//

        renderer_start(&renderer);

        shader_bind(&renderer.quad_shader);
        texture_bind(&tex, 0);
        shader_set_uniform_int(&renderer.quad_shader, "u_texture", 0);
        shader_set_uniform_mat4(&renderer.quad_shader, "u_model", mat4_translate(mat4_new(1), (vec3_t){-1, 0, -2}));

        vertex_array_bind(&quad_va);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        vertex_array_unbind();

        renderer_draw_model_3D(&renderer, &camera, &player_model, (vec3_t){0, 0, 0}, 1., quat_angle_axis(window.mouse_pos.x/2, (vec3_t){0, 1, 0}));
        //renderer_draw_skybox(&renderer, &skybox);

        renderer_end(&renderer);
    }

    renderer_exit(&renderer);
    window_exit(&window);
}
