#include "graphics/graphics.h"

f32 quad_vertices[] = {
    -1.0f, 1.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f,
    1.0f, -1.0f, 1.0f, 0.0f,

    -1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, -1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 1.0f };

void renderer_create(renderer_t* renderer)
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    /* glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); */

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    renderer->mesh_shader = (shader_t)
    {
        .v_path = "./res/shaders/mesh.vert",
        .f_path = "./res/shaders/mesh.frag"
    };
    shader_create(&renderer->mesh_shader);
    renderer->anim_mesh_shader = (shader_t)
    {
        .v_path = "./res/shaders/mesh.vert",
        .f_path = "./res/shaders/mesh.frag"
    };
    shader_create(&renderer->anim_mesh_shader);
    renderer->quad_shader = (shader_t)
    {
        .v_path = "./res/shaders/quad.vert",
        .f_path = "./res/shaders/quad.frag"
    };
    shader_create(&renderer->quad_shader);
    renderer->pp_shader = (shader_t)
    {
        .v_path = "./res/shaders/pp.vert",
        .f_path = "./res/shaders/pp.frag"
    };
    shader_create(&renderer->pp_shader);

    if (renderer->resolution.x == 0 || renderer->resolution.y == 0)
        renderer->resolution = renderer->window->size;

    renderer->scene_buffer = (framebuffer_t)
    {
        .color = true,
        .depth = true,
        .stencil = true,
        .resolution = renderer->resolution
    };
    renderer->pp_buffer = (framebuffer_t)
    {
        .color = true,
        .depth = true,
        .stencil = true,
        .resolution = renderer->resolution
    };
    framebuffer_create(&renderer->scene_buffer);
    framebuffer_create(&renderer->pp_buffer);

    vertex_array_create(&renderer->scene_buffer_va);
    vertex_array_create_vbo(&renderer->scene_buffer_va, quad_vertices, sizeof(quad_vertices), false);
    vertex_array_push_attribute_f(0, 2, 4 * sizeof(f32), 0);
    vertex_array_push_attribute_f(1, 2, 4 * sizeof(f32), 2 * sizeof(f32));
    vertex_array_unbind();
    vertex_array_unbind_buffers();
}

void renderer_start(renderer_t* renderer)
{
    int u = glGetError();
    if (u != 0)
    {
        printf(OPENGL_ERROR "%u\n", u);
        exit(-1);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, renderer->scene_buffer.fbo);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, renderer->scene_buffer.resolution.x, renderer->scene_buffer.resolution.y);

    glClearColor(renderer->clear_color.x, renderer->clear_color.y, renderer->clear_color.z, renderer->clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader_set_uniform_mat4(&renderer->mesh_shader, "u_view", renderer->view_mat);
    shader_set_uniform_mat4(&renderer->mesh_shader, "u_proj", renderer->proj_mat);

    shader_set_uniform_mat4(&renderer->quad_shader, "u_view", renderer->view_mat);
    shader_set_uniform_mat4(&renderer->quad_shader, "u_proj", renderer->proj_mat);

    shader_set_uniform_mat4(&renderer->anim_mesh_shader, "u_view", renderer->view_mat);
    shader_set_uniform_mat4(&renderer->anim_mesh_shader, "u_proj", renderer->proj_mat);
}

void renderer_end(renderer_t* renderer)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, renderer->scene_buffer.fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, renderer->pp_buffer.fbo);
    glBlitFramebuffer(0, 0, renderer->scene_buffer.resolution.x, renderer->scene_buffer.resolution.y, 0, 0, renderer->scene_buffer.resolution.x, renderer->scene_buffer.resolution.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, renderer->window->size.x, renderer->window->size.y);

    shader_bind(&renderer->pp_shader);
    vertex_array_bind(&renderer->scene_buffer_va);
    texture_bind(&renderer->pp_buffer.texture, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void renderer_clear(renderer_t* renderer)
{
}

void renderer_exit(renderer_t* renderer)
{
    shader_delete(&renderer->mesh_shader);
    shader_delete(&renderer->anim_mesh_shader);
    shader_delete(&renderer->pp_shader);
    shader_delete(&renderer->quad_shader);
    vertex_array_delete(&renderer->scene_buffer_va);
}

void renderer_draw_model_3D(renderer_t* renderer, camera_t* camera, model_3D_t* model, vec3_t pos, f32 size, vec4_t rotation)
{
    if (model->mesh_count != 0)
    {
        model->transform = mat4_translate(mat4_new(1), pos);
        if (rotation.x != 0 || rotation.y != 0 || rotation.z != 0)
            model->transform = mat4_multiply(model->transform, mat4_from_quat(rotation));
        if (size != 1)
            model->transform = mat4_scale(model->transform, size);

        shader_t* shader = &renderer->mesh_shader;

        shader_set_uniform_mat4(shader, "u_model", model->transform);

        for (u32 m = 0; m < model->mesh_count; m++)
        {
            for (u32 p = 0; p < model->meshes[m].primitive_count; p++)
            {
                shader_bind(shader);

                texture_bind(&model->meshes[m].primitives[p].material->diffuse_map, 0);
                shader_set_uniform_int(shader, "u_diffuse_map", 0);

                /*                 texture_bind(&model->meshes[m].primitives[p].material->orm_map, 1);
                                shader_set_uniform_int(shader, "u_orm_map", 1);

                                texture_bind(&model->meshes[m].primitives[p].material->normal_map, 2);
                                shader_set_uniform_int(shader, "u_normal_map", 2); */

                vertex_array_bind(&model->meshes[m].primitives[p].vertex_array);

                glDrawElements(GL_TRIANGLES, model->meshes[m].primitives[p].index_count, model->meshes[m].primitives[p].index_type, 0);

                texture_unbind(0);
                vertex_array_unbind();
            }
        }
        shader_unbind();
    }
}
