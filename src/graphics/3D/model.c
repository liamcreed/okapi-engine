#include "graphics/graphics.h"


void skeleton_create_hierarchy(mesh_joint_t *root)
{
    root->name = "bone";

    if (root->child_count > 0)
    {
        root->children = malloc(sizeof(mesh_joint_t) * root->child_count);
        for (int i = 0; i < root->child_count; i++)
        {
            root->children[i].parent = root;
            skeleton_create_hierarchy(&root->children[i]);
        }
    }
}

/* mesh_vertex_t vertices[] =
{
    (mesh_vertex_t){(vec3_t){0.5,0.5,0.0}, (vec3_t){0,0,0}, (vec2_t){1.0, 1.0}},
    (mesh_vertex_t){(vec3_t){0.5,-0.5,0.0}, (vec3_t){0,0,0}, (vec2_t){1.0, 0.0}},
    (mesh_vertex_t){(vec3_t){-0.5,-0.5,0.0}, (vec3_t){0,0,0}, (vec2_t){0.0, 0.0}},
    (mesh_vertex_t){(vec3_t){-0.5,0.5,0.0}, (vec3_t){0,0,0}, (vec2_t){0.0, 1.0}}
};  */

/* FILE *file;
   file = fopen(path, "rb");
   fseek(file, 0, SEEK_END);
   uint32_t length = ftell(file);
   fseek(file, 0, SEEK_SET);
   unsigned char *buffer = malloc(length);
   fread(buffer, 1, length, file);
   printf("%u\n", length); */

#define CGLTF_IMPLEMENTATION
#include "cgltf/cgltf.h"

void model_3D_create_from_file(model_3D_t *model, const char *path)
{
    cgltf_options gltf_options = {0};
    cgltf_data *gltf_data = NULL;
    cgltf_result gltf_result = cgltf_parse_file(&gltf_options, path, &gltf_data);
    if (gltf_result != cgltf_result_success)
    {
        printf(LOG_ERROR "[GLTF]: Failed to parse |%s|\n", path);
        exit(-1);
    }
    cgltf_load_buffers(&gltf_options, gltf_data, path);
    gltf_result = cgltf_validate(gltf_data);
    if (gltf_result != cgltf_result_success)
    {
        printf(LOG_ERROR "[GLTF]: binary data validation unsuccesful\n");
        exit(-1);
    }
    if (gltf_data->buffers->data == NULL)
    {
        printf(LOG_ERROR "[GLTF]: binary data is empty\n");
        exit(-1);
    }

    void *buffer = malloc(gltf_data->buffers->size);
    memcpy(buffer, gltf_data->buffers->data, gltf_data->buffers->size);
    model->material_count = gltf_data->materials_count;
    model->mesh_count = gltf_data->meshes_count;

    texture_t empty_color_map;
    unsigned char empty_color_data[] = {128, 128, 255, 255};
    texture_create_from_data(&empty_color_map, empty_color_data, (vec2_t){1, 1}, 4, false, true);

    texture_t empty_orm;
    unsigned char empty_orm_data[] = {255, 255, 0, 255};
    texture_create_from_data(&empty_orm, empty_orm_data, (vec2_t){1, 1}, 4, false, true);

    
    for (int mat = 0; mat < model->material_count; mat++)
    {
        model->materials[mat].name = strcat(gltf_data->materials[mat].name, "");
        printf(LOG_GLTF "Material: %s\n", model->materials[mat].name);
        // Base color
        if (gltf_data->materials[mat].pbr_metallic_roughness.base_color_texture.texture != NULL)
        {
            char *image_path = get_full_path_from_other(path, gltf_data->materials[mat].pbr_metallic_roughness.base_color_texture.texture->image->uri);
            printf("%s\n", image_path);
            texture_create_from_png(&model->materials[mat].diffuse_map, image_path, true, true);
            free(image_path);
        }
        else
        {
            model->materials[mat].diffuse_map = empty_color_map;
            printf(LOG_WARNING "[GLTF]: No base color texture in material!\n");
        }

        model->materials[mat].orm_map = empty_orm;

        model->materials[mat].normal_map = empty_color_map;
    }

    for (int m = 0; m < model->mesh_count; m++)
    {
        model->meshes[m].primitive_count = 1;
        for (int p = 0; p < model->meshes[m].primitive_count; p++)
        {

            bool material_found;
            for (int mat = 0; mat < model->material_count; mat++)
            {
                if (gltf_data->meshes[m].primitives[p].material->name == model->materials[mat].name)
                {
                    model->meshes[m].primitives[p].material = &model->materials[mat];
                    material_found = true;
                }
            }
            if (!material_found)
            {
                model->meshes[m].primitives[p].material = &model->materials[0];
            }

            model->meshes[m].primitives[p].index_count = gltf_data->meshes[m].primitives[p].indices->count;
            model->meshes[m].primitives[p].index_type = GL_UNSIGNED_INT;

            size_t vertices_offset = gltf_data->meshes[m].primitives[p].attributes->data->buffer_view->offset;

            void *vertices = buffer + vertices_offset;

            size_t indices_size = gltf_data->meshes[m].primitives[p].indices->buffer_view->size;
            size_t indices_offset = gltf_data->meshes[m].primitives[p].indices->buffer_view->offset;
            void *indices = buffer + indices_offset;

            uint32_t index_type = gltf_data->meshes[m].primitives[p].indices->component_type;
            if (index_type == cgltf_component_type_r_8)
                model->meshes[m].primitives[p].index_type = GL_UNSIGNED_BYTE;
            else if (index_type == cgltf_component_type_r_16)
                model->meshes[m].primitives[p].index_type = GL_SHORT;
            else if (index_type == cgltf_component_type_r_16u)
                model->meshes[m].primitives[p].index_type = GL_UNSIGNED_SHORT;
            else if (index_type == cgltf_component_type_r_32u)
                model->meshes[m].primitives[p].index_type = GL_UNSIGNED_INT;
            else if (index_type == cgltf_component_type_r_32f)
                model->meshes[m].primitives[p].index_type = GL_FLOAT;

            size_t vertices_size = indices_offset - vertices_offset;

            vertex_array_create(&model->meshes[m].primitives[p].vertex_array);
            vertex_array_create_vbo(&model->meshes[m].primitives[p].vertex_array, vertices, vertices_size, false);

            model->meshes[m].primitives[p].attribute_count = gltf_data->meshes[m].primitives[p].attributes_count;

            for (int atr = 0; atr < model->meshes[m].primitives[p].attribute_count; atr++)
            {
                size_t atr_offset = gltf_data->meshes[m].primitives[p].attributes[atr].data->buffer_view->offset - vertices_offset;
                size_t atr_stride = gltf_data->meshes[m].primitives[p].attributes[atr].data->buffer_view->stride;

                char *atr_name = gltf_data->meshes[m].primitives[p].attributes[atr].name;

                if (strcmp(atr_name, "POSITION") == 0)
                {
                    vertex_array_push_attribute_f(0, 3, atr_stride, atr_offset);
                }
                else if (strcmp(atr_name, "NORMAL") == 0)
                {
                    vertex_array_push_attribute_f(1, 3, atr_stride, atr_offset);
                }
                else if (strcmp(atr_name, "TEXCOORD_0") == 0)
                {
                    vertex_array_push_attribute_f(2, 2, atr_stride, atr_offset);
                }
            }
            vertex_array_create_ibo(&model->meshes[m].primitives[p].vertex_array, indices, indices_size, false);
            vertex_array_unbind();
            vertex_array_unbind_buffers();
        }
    }
    free(gltf_data);
    free(buffer);
    printf("created model\n");
}

void model_3D_delete(model_3D_t *model)
{
    for (int m = 0; m < model->material_count; m++)
    {
        texture_delete(&model->materials->diffuse_map);
        texture_delete(&model->materials->orm_map);
        texture_delete(&model->materials->normal_map);
    }
    for (uint32_t m = 0; m < model->mesh_count; m++)
    {
        for (uint32_t p = 0; p < model->meshes[m].primitive_count; p++)
        {
            free(model->meshes[m].primitives[p].vertices);
            free(model->meshes[m].primitives[p].indices);
        }
    }
}
