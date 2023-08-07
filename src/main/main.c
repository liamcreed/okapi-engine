#include "api.h"

int main(void)
{
    mat4_inverse(mat4_new(1));
    window_t window =
    {
        .size.x = 800 * 2,
        .size.y = 600 * 2,
        .vsync = true,
        .title = "okapi"
    };

    window_create(&window);

    renderer_t renderer =
    {
        .window = &window,
        .clear_color = (vec4_t){.7, .9, 1, .5},
        .resolution = (vec2_t){200, 200 / window.aspect}
    };

    renderer_create(&renderer);

    camera_t camera = {};
    camera.fov = 45;
    camera.near = 0.1;
    camera.far = 100;
    camera_create_perspective(&camera);

    model_3D_t player_model = {};
    model_3D_create_from_file(&player_model, "./res/3D/knight/knight.gltf");

    texture_t tex = {};
    tex.sRGB = true;
    texture_create_from_bin(&tex, "./res/textures/tilemap.okpt");

    f32 vertices[] =
    {
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f
    };
    u32 indices[] =
    {
        0, 1, 3,
        1, 2, 3
    };

    vertex_array_t quad_va = {};
    vertex_array_create(&quad_va);
    vertex_array_create_vbo(&quad_va, vertices, sizeof(vertices), false);
    vertex_array_push_attribute_f(0, 3, 5 * sizeof(f32), 0);
    vertex_array_push_attribute_f(2, 2, 5 * sizeof(f32), 3 * sizeof(f32));
    vertex_array_create_ibo(&quad_va, indices, sizeof(indices), false);

    while (!window.closed)
    {
        window_update(&window);
        if (camera.orthographic)
            renderer.proj_mat = mat4_ortho_aspect(window.aspect, camera.orthographic_size, camera.near, camera.far);
        else
            renderer.proj_mat = mat4_perspective(camera.fov, window.aspect, camera.near, camera.far);

        static f32 cam_z = 4;
        if (key_pressed(&window, KEY_W))
            cam_z -= 1 * window.dt;
        else if (key_pressed(&window, KEY_S))
            cam_z += 1 * window.dt;

        renderer.view_mat = mat4_look_at((vec3_t) { 0, 0, cam_z }, (vec3_t) { 0, 0, 0 }, (vec3_t) { 0, 1, 0 });

        //-------------------------------------------------//

        renderer_start(&renderer);

        shader_bind(&renderer.quad_shader);
        texture_bind(&tex, 0);
        shader_set_uniform_int(&renderer.quad_shader, "u_texture", 0);
        shader_set_uniform_mat4(&renderer.quad_shader, "u_model", mat4_rotate_z(mat4_scale(mat4_new(1), 6), glfwGetTime() * 10));

        vertex_array_bind(&quad_va);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        vertex_array_unbind();

        renderer_draw_model_3D(&renderer, &camera, &player_model, (vec3_t) { 0, 0, 1 }, 1., quat_angle_axis(window.mouse_pos.x / 2, (vec3_t) { 0, 1, 0 }));

        renderer_end(&renderer);
    }

    renderer_exit(&renderer);
    window_exit(&window);
}
