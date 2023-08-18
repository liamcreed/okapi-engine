#include "graphics/graphics.h"

f32 scene_vertices[] =
{
    -1.0f, 1.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f,
    1.0f, -1.0f, 1.0f, 0.0f,

    -1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, -1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 1.0f
};

f32 quad_vertices[] =
{
    0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.0f, 0.0f, 1.0f
};
u32 quad_indices[] =
{
    0, 1, 3,
    1, 2, 3
};


void renderer_create(renderer_t* renderer)
{
    //opengl settings
    GL(glEnable(GL_DEPTH_TEST));
    GL(glDepthFunc(GL_LESS));
    GL(glEnable(GL_CULL_FACE));

    GL(glEnable(GL_BLEND));
    GL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    //creating shaders
    renderer->mesh_shader = (shader_t)
    {
        .v_path = "./assets/shaders/mesh.vert",
        .f_path = "./assets/shaders/mesh.frag"
    };
    shader_create(&renderer->mesh_shader);

    renderer->quad_shader = (shader_t)
    {
        .v_path = "./assets/shaders/quad.vert",
        .f_path = "./assets/shaders/quad.frag"
    };
    shader_create(&renderer->quad_shader);

    //creating vao for quads
    vertex_array_create(&renderer->quad_va);
    vertex_array_create_vbo(&renderer->quad_va, quad_vertices, sizeof(quad_vertices), false);
    vertex_array_push_attribute_f(0, 3, 5 * sizeof(f32), 0);
    vertex_array_push_attribute_f(2, 2, 5 * sizeof(f32), 3 * sizeof(f32));
    vertex_array_create_ibo(&renderer->quad_va, quad_indices, sizeof(quad_indices), false);
    vertex_array_unbind();
    vertex_array_unbind_buffers();

    u8 white_tex_data[] = { 255,255,255,255 };
    renderer->white_tex = (texture_t)
    {
        .data = white_tex_data,
        .width = 1,
        .height = 1,
        .channel_count = 4,
    };
    texture_create(&renderer->white_tex);

    //postprocessing
    renderer->pp_shader = (shader_t)
    {
        .v_path = "./assets/shaders/pp.vert",
        .f_path = "./assets/shaders/pp.frag"
    };
    shader_create(&renderer->pp_shader);

    if (renderer->width == 0)
        renderer->width == renderer->window->width;
    if (renderer->height == 0)
        renderer->height == renderer->window->height;

    //creating framebuffer for the scene
    renderer->scene_buffer = (framebuffer_t)
    {
        .color = true,
        .depth = true,
        .texture.width = renderer->width,
        .texture.height = renderer->height,
    };

    framebuffer_create(&renderer->scene_buffer);

    vertex_array_create(&renderer->scene_buffer_va);
    vertex_array_create_vbo(&renderer->scene_buffer_va, scene_vertices, sizeof(scene_vertices), false);
    vertex_array_push_attribute_f(0, 2, 4 * sizeof(f32), 0);
    vertex_array_push_attribute_f(1, 2, 4 * sizeof(f32), 2 * sizeof(f32));
    vertex_array_unbind();
    vertex_array_unbind_buffers();
}

void renderer_start(renderer_t* renderer)
{
    glEnable(GL_DEPTH_TEST);
    GL(glBindFramebuffer(GL_FRAMEBUFFER, renderer->scene_buffer.fbo));
    GL(glViewport(0, 0, renderer->scene_buffer.texture.width, renderer->scene_buffer.texture.height));
    GL(glClearColor(renderer->clear_color.x, renderer->clear_color.y, renderer->clear_color.z, renderer->clear_color.w));
    GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    shader_set_uniform_mat4(&renderer->quad_shader, "u_view", renderer->view_mat);
    shader_set_uniform_mat4(&renderer->quad_shader, "u_proj", renderer->proj_mat);

    shader_set_uniform_mat4(&renderer->mesh_shader, "u_view", renderer->view_mat);
    shader_set_uniform_mat4(&renderer->mesh_shader, "u_proj", renderer->proj_mat);
}

void renderer_end(renderer_t* renderer)
{
    //drawing quad with the scene to the screen
    GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    GL(glDisable(GL_DEPTH_TEST));

    GL(glClearColor(1.0f, 1.0f, 1.0f, 1.0f));
    GL(glClear(GL_COLOR_BUFFER_BIT));
    GL(glViewport(0, 0, renderer->window->width, renderer->window->height));

    //drawing the scene quad
    shader_bind(&renderer->pp_shader);
    vertex_array_bind(&renderer->scene_buffer_va);
    texture_bind(&renderer->scene_buffer.texture, 0);
    GL(glDrawArrays(GL_TRIANGLES, 0, 6));
}

void renderer_clear(renderer_t* renderer)
{
}

void renderer_exit(renderer_t* renderer)
{
    shader_delete(&renderer->mesh_shader);
    shader_delete(&renderer->pp_shader);
    shader_delete(&renderer->quad_shader);
    vertex_array_delete(&renderer->quad_va);
    vertex_array_delete(&renderer->scene_buffer_va);
}


void renderer_draw_model_3D(renderer_t* renderer, model_3D_t* model, vec3_t position, f32 size, vec4_t rotation)
{
    if (model->mesh_count != 0)
    {
        mat4_t transform = mat4_translate(mat4_new(1), position);
        if (rotation.x != 0 || rotation.y != 0 || rotation.z != 0)
            transform = mat4_multiply(transform, mat4_from_quat(rotation));
        if (size != 0 || size != 1)
            transform = mat4_scale(transform, (vec3_t) { size, size, size });

        shader_t* shader = &renderer->mesh_shader;
        shader_set_uniform_mat4(shader, "u_model", transform);

        if (model->armature.joint_count > 0)
        {
            armature_update_transforms(&model->armature);
            shader_set_uniform_mat4_arr(shader, "joint_matrices", model->armature.joint_matrices, model->armature.joint_count);
            shader_set_uniform_int(shader, "u_skinning", 1);
        }
        else
        {
            shader_set_uniform_int(shader, "u_skinning", 0);
        }

        for (i32 m = 0; m < model->mesh_count; m++)
        {
            for (i32 p = 0; p < model->meshes[m].primitive_count; p++)
            {
                shader_bind(shader);

                texture_bind(&model->materials[model->meshes[m].primitives[p].material_index].diffuse_map, 0);
                shader_set_uniform_int(shader, "u_diffuse_map", 0);

                shader_set_uniform_vec4(shader, "u_color", model->materials[model->meshes[m].primitives[p].material_index].color);

                vertex_array_bind(&model->meshes[m].primitives[p].vertex_array);

                GL(glDrawElements(GL_TRIANGLES, model->meshes[m].primitives[p].index_count, model->meshes[m].primitives[p].index_type, 0));

                GL_check_error();
                texture_unbind(0);
                vertex_array_unbind();
            }
        }
        shader_unbind();
    }
}

void renderer_draw_quad(renderer_t* renderer, texture_t* texture, vec4_t color, vec3_t position, vec3_t size, vec4_t rotation)
{
    GL(glDisable(GL_CULL_FACE));
    shader_bind(&renderer->quad_shader);

    if (texture == NULL)
        texture_bind(&renderer->white_tex, 0);
    else
        texture_bind(texture, 0);

    shader_set_uniform_int(&renderer->quad_shader, "u_texture", 0);

    mat4_t transform = mat4_translate(mat4_new(1), position);
    if (rotation.x != 0 || rotation.y != 0 || rotation.z != 0)
        transform = mat4_multiply(transform, mat4_from_quat(rotation));
    if (size.x != 0 || size.y != 0)
        transform = mat4_scale(transform, size);

    shader_set_uniform_mat4(&renderer->quad_shader, "u_model", transform);
    shader_set_uniform_vec4(&renderer->quad_shader, "u_color", color);

    vertex_array_bind(&renderer->quad_va);
    GL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
    vertex_array_unbind();
    GL(glEnable(GL_CULL_FACE));
}