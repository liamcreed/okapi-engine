#include "graphics/graphics.h"
float quad_vertices[] = {
    -1.0f, 1.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f,
    1.0f, -1.0f, 1.0f, 0.0f,

    -1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, -1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 1.0f};

void renderer_create(renderer_t *renderer, window_t *window, vec4_t clear_color, float resolution_scale)
{
    renderer->window = window;
    renderer->clear_color = clear_color;
    renderer->resolution_scale = resolution_scale;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    /* glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); */

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader_create(&renderer->mesh_shader, "./res/shaders/mesh.vert", "./res/shaders/mesh.frag");
    shader_create(&renderer->skybox_shader, "./res/shaders/skybox.vert", "./res/shaders/skybox.frag");
    shader_create(&renderer->quad_shader, "res/shaders/quad.vert", "res/shaders/quad.frag");

    framebuffer_create(&renderer->scene_buffer, (vec2_t){renderer->window->size.x * resolution_scale, renderer->window->size.y * resolution_scale}, false);
    vertex_array_create(&renderer->scene_buffer_va);
    vertex_array_create_vbo(&renderer->scene_buffer_va, quad_vertices, sizeof(quad_vertices), false);
    vertex_array_push_attribute(0, 2, 4 * sizeof(float), 0);
    vertex_array_push_attribute(1, 2, 4 * sizeof(float), 2 * sizeof(float));
    vertex_array_unbind();
    vertex_array_unbind_buffers();
    shader_create(&renderer->pp_shader, "./res/shaders/pp.vert", "./res/shaders/pp.frag");
}

void renderer_start(renderer_t *renderer)
{
    framebuffer_bind(&renderer->scene_buffer);
    renderer_clear(renderer);

    shader_set_uniform_mat4(&renderer->mesh_shader, "u_view", renderer->view_mat);
    shader_set_uniform_mat4(&renderer->mesh_shader, "u_proj", renderer->proj_mat);

    mat4_t view = renderer->view_mat;
    view.data[0][3] = 0;
    view.data[1][3] = 0;
    view.data[2][3] = 0;
    view.data[3][3] = 1;

    shader_set_uniform_mat4(&renderer->skybox_shader, "u_view", view);
    shader_set_uniform_mat4(&renderer->skybox_shader, "u_proj", renderer->proj_mat);
}

void renderer_end(renderer_t *renderer)
{
    framebuffer_unbind();
    glDisable(GL_DEPTH_TEST);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, renderer->window->size.x, renderer->window->size.y);

    shader_bind(&renderer->pp_shader);
    vertex_array_bind(&renderer->scene_buffer_va);
    texture_bind(&renderer->scene_buffer.texture, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void renderer_clear(renderer_t *renderer)
{
    glClearColor(renderer->clear_color.x, renderer->clear_color.y, renderer->clear_color.z, renderer->clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int u = glGetError();
    if (u != 0)
    {
        printf(OPENGL_ERROR "%u\n", u);
    }
}



void renderer_exit(renderer_t *renderer)
{
    shader_delete(&renderer->mesh_shader);
    shader_delete(&renderer->pp_shader);
    shader_delete(&renderer->quad_shader);
    vertex_array_delete(&renderer->scene_buffer_va);
}

void renderer_draw_model_3D(renderer_t *renderer, camera_t *camera, model_3D_t *model, vec3_t pos, float size, vec3_t rotation)
{
    model->transform = mat4_translate(mat4_new(1), pos);
     if (rotation.x != 0 && rotation.y != 0 && rotation.z != 0)
        model->transform = mat4_rotate(model->transform, rotation); 
    else if (rotation.x != 0)
        model->transform = mat4_rotate_x(model->transform, rotation.x);
    else if (rotation.y != 0)
        model->transform = mat4_rotate_y(model->transform, rotation.y);
    else if (rotation.z != 0)
        model->transform = mat4_rotate_z(model->transform, rotation.z);  
    model->transform = mat4_rotate(model->transform, rotation); 
    if (size != 1)
        model->transform = mat4_scale(model->transform, size);

    shader_set_uniform_mat4(&renderer->mesh_shader, "u_model", model->transform);

    shader_bind(&renderer->mesh_shader);
    for (int m = 0; m < model->mesh_count; m++)
    {
        for (int p = 0; p < model->meshes[m].primitive_count; p++)
        {
            shader_set_uniform_vec3(&renderer->mesh_shader, "cam_pos", camera->pos);

            texture_bind(&model->meshes[m].primitives[p].material->diffuse_map, 0);
            shader_set_uniform_int(&renderer->mesh_shader, "diffuse_map", 0);

            texture_bind(&model->meshes[m].primitives[p].material->orm_map, 1);
            shader_set_uniform_int(&renderer->mesh_shader, "orm_map", 1);

            texture_bind(&model->meshes[m].primitives[p].material->normal_map, 2);
            shader_set_uniform_int(&renderer->mesh_shader, "normal_map", 2);

            vertex_array_bind(&model->meshes[m].primitives[p].vertex_array);

            glDrawElements(GL_TRIANGLES, model->meshes[m].primitives[p].index_count, model->meshes[m].primitives[p].index_type, 0);

            texture_unbind(0);
            vertex_array_unbind();
        }
    }
    shader_unbind();
}

void renderer_draw_skybox(renderer_t *renderer, skybox_t *skybox)
{
    glDepthFunc(GL_LEQUAL);
    shader_bind(&renderer->skybox_shader);
    vertex_array_bind(&skybox->va);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->cubemap.id);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    vertex_array_unbind();
    glDepthFunc(GL_LESS);
}