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
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // create white texture
    unsigned char white[4] = {255, 255, 255, 255};
    texture_t white_tex;
    texture_create_from_data(&white_tex, white, (vec2_t){1, 1}, false);
    renderer->quad_textures[0] = white_tex;

    // pre calculate indices
    int index_offset = 0;
    for (int i = 0; i < MAX_QUAD_COUNT * 6; i += 6)
    {
        renderer->quad_indices[i + 0] = 0 + index_offset;
        renderer->quad_indices[i + 1] = 1 + index_offset;
        renderer->quad_indices[i + 2] = 2 + index_offset;
        renderer->quad_indices[i + 3] = 2 + index_offset;
        renderer->quad_indices[i + 4] = 3 + index_offset;
        renderer->quad_indices[i + 5] = 0 + index_offset;

        index_offset += 4;
    }

    // create vertexarray
    vertex_array_create(&renderer->quad_vertex_array);
    vertex_array_create_vbo(&renderer->quad_vertex_array, renderer->quad_indices, sizeof(uint32_t) * MAX_QUAD_COUNT * 6, true);
    vertex_array_push_attribute(0, 3, sizeof(quad_vertex_t), offsetof(quad_vertex_t, pos));
    vertex_array_push_attribute(1, 4, sizeof(quad_vertex_t), offsetof(quad_vertex_t, color));
    vertex_array_push_attribute(2, 2, sizeof(quad_vertex_t), offsetof(quad_vertex_t, uv));
    vertex_array_push_attribute(3, 1, sizeof(quad_vertex_t), offsetof(quad_vertex_t, tex_index));
    vertex_array_create_ibo(&renderer->quad_vertex_array, renderer->quad_indices, sizeof(uint32_t) * MAX_QUAD_COUNT * 6, true);
    vertex_array_unbind();
    vertex_array_unbind_buffers();

    shader_create(&renderer->quad_shader, "res/shaders/quad_shader.vert", "res/shaders/quad_shader.frag");

    int samplers[MAX_TEXTURE_COUNT];
    for (uint32_t i = 0; i < MAX_TEXTURE_COUNT; i++)
    {
        samplers[i] = i;
    }

    shader_set_uniform_int_arr(&renderer->quad_shader, "u_textures", samplers, MAX_TEXTURE_COUNT);

    renderer->quad_index_count = 0;
    renderer->quad_texture_count = 1;

    shader_create(&renderer->mesh_shader, "./res/shaders/mesh.vert", "./res/shaders/mesh.frag");
    shader_create(&renderer->skybox_shader, "./res/shaders/skybox.vert", "./res/shaders/skybox.frag");

    framebuffer_create(&renderer->scene_buffer, (vec2_t){renderer->window->size.x * resolution_scale, renderer->window->size.y * resolution_scale}, false);
    vertex_array_create(&renderer->scene_buffer_va);
    vertex_array_create_vbo(&renderer->scene_buffer_va, quad_vertices, sizeof(quad_vertices), false);
    vertex_array_push_attribute(0, 2, 4 * sizeof(float), 0);
    vertex_array_push_attribute(1, 2, 4 * sizeof(float), 2 * sizeof(float));
    vertex_array_unbind();
    vertex_array_unbind_buffers();
    shader_create(&renderer->pp_shader, "./res/shaders/pp.vert", "./res/shaders/pp.frag");
}

void renderer_start(renderer_t *renderer, camera_t *camera)
{
    framebuffer_bind(&renderer->scene_buffer);
    renderer_clear(renderer);

    if (camera->orthographic)
        renderer->proj_mat = mat4_ortho_aspect(renderer->window->aspect, camera->orthographic_size, camera->near, camera->far);
    else
        renderer->proj_mat = mat4_perspective(camera->fov, renderer->window->aspect, camera->near, camera->far);

    renderer->view_mat = mat4_look_at((camera->pos), vec3_add(camera->pos, camera->front), camera->up);

    shader_set_uniform_mat4(&renderer->quad_shader, "u_view_mat", renderer->view_mat);
    shader_set_uniform_mat4(&renderer->quad_shader, "u_proj_mat", renderer->proj_mat);

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

    if (glGetError() != 0)
    {
        printf(OPENGL_ERROR "%u\n", glGetError());
    }
}

void renderer_batch_start(renderer_t *renderer)
{
    shader_bind(&renderer->quad_shader);

    renderer->quad_vertices_p = renderer->quad_vertices;
    renderer->quad_index_count = 0;
    renderer->quad_texture_count = 1;
}

void renderer_batch_end(renderer_t *renderer)
{
    size_t size = (uint8_t *)renderer->quad_vertices_p - (uint8_t *)renderer->quad_vertices;
    vertex_array_bind(&renderer->quad_vertex_array);
    vertex_array_bind_buffers(&renderer->quad_vertex_array);

    vertex_array_push_vertex_data(size, renderer->quad_vertices);

    for (uint32_t i = 0; i < renderer->quad_texture_count; i++)
    {
        texture_bind(&renderer->quad_textures[i], i);
    }

    shader_bind(&renderer->quad_shader);

    glDrawElements(GL_TRIANGLES, renderer->quad_index_count, GL_UNSIGNED_INT, 0);

    for (uint32_t i = 0; i < renderer->quad_texture_count; i++)
    {
        texture_unbind(i);
    }
    vertex_array_unbind();
    vertex_array_unbind_buffers();

    shader_unbind();
}

void renderer_exit(renderer_t *renderer)
{
    for (uint32_t i = 0; i < renderer->quad_texture_count; i++)
    {
        texture_delete(&renderer->quad_textures[i]);
    }

    shader_delete(&renderer->quad_shader);
    vertex_array_delete(&renderer->quad_vertex_array);
}

void renderer_draw_quad(
    renderer_t *renderer,
    texture_t *texture,
    vec3_t pos,
    vec3_t size,
    vec4_t color)
{
    if (renderer->quad_index_count >= MAX_QUAD_COUNT * 6 || renderer->quad_texture_count >= MAX_TEXTURE_COUNT)
    {
        renderer_batch_end(renderer);
        renderer_batch_start(renderer);
    }

    float tex_index = 0.0f;

    if (texture != NULL)
    {
        for (uint32_t i = 0; i < renderer->quad_texture_count; i++)
        {
            if (renderer->quad_textures[i].id == texture->id)
            {
                tex_index = (float)i;
                break;
            }
        }

        if (tex_index == 0.0f)
        {
            tex_index = (float)renderer->quad_texture_count;
            renderer->quad_textures[renderer->quad_texture_count] = *texture;
            renderer->quad_texture_count += 1;
        }
    }

    renderer->quad_vertices_p->pos = (vec3_t){pos.x - (size.x / 2), pos.y - (size.y / 2), pos.z};
    renderer->quad_vertices_p->color = color;
    renderer->quad_vertices_p->uv = (vec2_t){0.0f, 0.0f};
    renderer->quad_vertices_p->tex_index = tex_index;
    renderer->quad_vertices_p++;

    renderer->quad_vertices_p->pos = (vec3_t){pos.x + (size.x / 2), pos.y - (size.y / 2), pos.z};
    renderer->quad_vertices_p->color = color;
    renderer->quad_vertices_p->uv = (vec2_t){1.0f, 0.0f};
    renderer->quad_vertices_p->tex_index = tex_index;
    renderer->quad_vertices_p++;

    renderer->quad_vertices_p->pos = (vec3_t){pos.x + (size.x / 2), pos.y + (size.y / 2), pos.z};
    renderer->quad_vertices_p->color = color;
    renderer->quad_vertices_p->uv = (vec2_t){1.0f, 1.0f};
    renderer->quad_vertices_p->tex_index = tex_index;
    renderer->quad_vertices_p++;

    renderer->quad_vertices_p->pos = (vec3_t){pos.x - (size.x / 2), pos.y + (size.y / 2), pos.z};
    renderer->quad_vertices_p->color = color;
    renderer->quad_vertices_p->uv = (vec2_t){0.0f, 1.0f};
    renderer->quad_vertices_p->tex_index = tex_index;
    renderer->quad_vertices_p++;
    renderer->quad_index_count += 6;
}

void renderer_draw_sub_texture(
    renderer_t *renderer,
    texture_t *texture,
    sub_texture_t *sub_texture,
    vec3_t pos,
    vec3_t size,
    vec4_t color)
{
    if (renderer->quad_index_count >= MAX_QUAD_COUNT * 6 || renderer->quad_texture_count >= MAX_TEXTURE_COUNT)
    {
        renderer_batch_end(renderer);
        renderer_batch_start(renderer);
    }
    float tex_index = 0.0f;

    for (uint32_t i = 0; i < renderer->quad_texture_count; i++)
    {
        if (renderer->quad_textures[i].id == texture->id)
        {
            tex_index = (float)i;
            break;
        }
    }

    if (tex_index == 0.0f)
    {
        tex_index = (float)renderer->quad_texture_count;
        renderer->quad_textures[renderer->quad_texture_count] = *texture;
        renderer->quad_texture_count += 1;
    }

    renderer->quad_vertices_p->pos = (vec3_t){pos.x - (size.x / 2), pos.y - (size.y / 2), pos.z};
    renderer->quad_vertices_p->color = color;
    renderer->quad_vertices_p->uv = sub_texture->uv[0];
    renderer->quad_vertices_p->tex_index = tex_index;
    renderer->quad_vertices_p++;

    renderer->quad_vertices_p->pos = (vec3_t){pos.x + (size.x / 2), pos.y - (size.y / 2), pos.z};
    renderer->quad_vertices_p->color = color;
    renderer->quad_vertices_p->uv = sub_texture->uv[1];
    renderer->quad_vertices_p->tex_index = tex_index;
    renderer->quad_vertices_p++;

    renderer->quad_vertices_p->pos = (vec3_t){pos.x + (size.x / 2), pos.y + (size.y / 2), pos.z};
    renderer->quad_vertices_p->color = color;
    renderer->quad_vertices_p->uv = sub_texture->uv[2];
    renderer->quad_vertices_p->tex_index = tex_index;
    renderer->quad_vertices_p++;

    renderer->quad_vertices_p->pos = (vec3_t){pos.x - (size.x / 2), pos.y + (size.y / 2), pos.z};
    renderer->quad_vertices_p->color = color;
    renderer->quad_vertices_p->uv = sub_texture->uv[3];
    renderer->quad_vertices_p->tex_index = tex_index;
    renderer->quad_vertices_p++;
    renderer->quad_index_count += 6;
}

void renderer_draw_model_3D(renderer_t *renderer, camera_t* camera, model_3D_t *model, vec3_t pos, float size, vec3_t rotation)
{
    model->transform = mat4_translate(mat4_new(1), pos);
    if (size != 1)
        model->transform = mat4_scale(model->transform, size);
    if (rotation.x != 0)
        model->transform = mat4_rotate_x(model->transform, rotation.x);
    if (rotation.y != 0)
        model->transform = mat4_rotate_y(model->transform, rotation.y);
    if (rotation.z != 0)
        model->transform = mat4_rotate_z(model->transform, rotation.z);

    shader_set_uniform_mat4(&renderer->mesh_shader, "u_model", model->transform);

    shader_bind(&renderer->mesh_shader);
    for (int m = 0; m < model->mesh_count; m++)
    {
        for (int p = 0; p < model->meshes[m].primitive_count; p++)
        {
            shader_set_uniform_vec3(&renderer->mesh_shader, "cam_pos", camera->pos);

            texture_bind(&model->meshes[m].primitives[p].material->diffuse_map, 0);
            shader_set_uniform_int(&renderer->mesh_shader, "diffuse_map", 0);

            texture_bind(&model->meshes[m].primitives[p].material->specular_map, 1);
            shader_set_uniform_int(&renderer->mesh_shader, "specular_map", 1);

    /*         texture_bind(&model->meshes[m].primitives[p].material->normal_map, 1);
            shader_set_uniform_int(&renderer->mesh_shader, "normal_map", 1);  */

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