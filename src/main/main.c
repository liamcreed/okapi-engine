#include "api.h"

window_t window;
renderer_t renderer;

#include "stb_image/stb_image.h"

int main(void)
{
    window_create(&window, (vec2_t){800 * 2, 600 * 2}, "okapi", 1);
    renderer_create(&renderer, &window, (vec4_t){.7, .9, 1, .5}, 1);

    camera_t camera;

    model_3D_t scene;
    model_3D_create_from_file(&scene, "./res/3D/stonehenge/stonehenge.gltf");

    model_3D_t clone_trooper;
    model_3D_create_from_file(&clone_trooper, "./res/3D/clone/clone.gltf");

    camera_create_perspective(&camera, 45, 0.1, 2000);
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

    vec3_t player_pos = (vec3_t){0, 0, 2.5};
    vec3_t player_rot = (vec3_t){0, 180, 0};

    float last_x = window.size.x / 2;
    float last_y = window.size.y / 2;
    bool first_mouse = true;

    while (!window.closed)
    {
        window_update(&window);

        //-------------------------------------------------//

        float x_pos = window.mouse_pos.x;
        float y_pos = window.mouse_pos.y;

        if (first_mouse)
        {
            last_x = x_pos;
            last_y = y_pos;
            first_mouse = false;
        }

        float x_offset = x_pos - last_x;
        float y_offset = last_y - y_pos;
        last_x = x_pos;
        last_y = y_pos;

        const float sensitivity = 0.1f;
        x_offset *= sensitivity;
        y_offset *= sensitivity;

        camera.pitch += y_offset;
        camera.yaw += x_offset;

        static float radius = 5;
        if (key_pressed(&window, KEY_UP))
            radius += 10 * window.dt;
        else if (key_pressed(&window, KEY_DOWN))
            radius -= 10 * window.dt;

        if (camera.pitch > 40 / radius)
            camera.pitch = 40 / radius;
        if (camera.pitch < -70)
            camera.pitch = -70;

        float x;
        float z;
        if (key_pressed(&window, KEY_A))
            x = -1;
        else if (key_pressed(&window, KEY_D))
            x = 1;
        else
            x = 0;
        if (key_pressed(&window, KEY_W))
            z = -1;
        else if (key_pressed(&window, KEY_S))
            z = 1;
        else
            z = 0;

        if (x !=0 && z != 0)
        {
            x *= cos(radians(45));
            z *= cos(radians(45));
        }

        if (x != 0 || z != 0)
            player_rot.y = lerp(player_rot.y, camera.yaw + 180, 0.2);

        float speed = radius * window.dt;
        if (key_pressed(&window, KEY_LEFT_SHIFT))
            speed *= 2;

        quat_t cam_quat = quat_from_euler((vec3_t){0, -camera.yaw, 0});
        vec3_t speed_vec = (vec3_t){x * speed, 0, z * speed};
        vec3_t movement = vec3_multiply_quat((speed_vec), cam_quat);
        player_pos = vec3_add(player_pos, movement);

        float hor_dist = radius * cos(radians(camera.pitch));
        float vert_dist = radius * sin(radians(camera.pitch));
        float cam_x_offset = hor_dist * sin(radians(180 - camera.yaw));
        float cam_z_offset = hor_dist * cos(radians(180 - camera.yaw));

        camera.pos.x = player_pos.x - cam_x_offset;
        camera.pos.y = lerp(camera.pos.y, player_pos.y - vert_dist, 0.8);
        camera.pos.z = player_pos.z - cam_z_offset;

        //-------------------------------------------------//

        if (camera.orthographic)
            renderer.proj_mat = mat4_ortho_aspect(window.aspect, camera.orthographic_size, camera.near, camera.far);
        else
            renderer.proj_mat = mat4_perspective(camera.fov, window.aspect, camera.near, camera.far);

        renderer.view_mat = mat4_look_at(camera.pos, (vec3_t){player_pos.x, player_pos.y + 0.1, player_pos.z}, camera.up);
        renderer.view_mat = mat4_translate(renderer.view_mat, (vec3_t){0, -1, 0});

        //-------------------------------------------------//

        renderer_start(&renderer);

        renderer_draw_model_3D(&renderer, &camera, &scene, (vec3_t){}, 1, (vec3_t){});
        renderer_draw_model_3D(&renderer, &camera, &clone_trooper, player_pos, 1, player_rot);
        renderer_draw_skybox(&renderer, &skybox);

        renderer_end(&renderer);
    }

    renderer_exit(&renderer);
    window_exit(&window);
}
