#include "graphics/graphics.h"

#define CGLTF_IMPLEMENTATION
#include "cgltf/cgltf.h"

char *get_full_path(const char *path, char *file_name)
{
    int path_length = strlen(path);
    int l;

    for (int i = 0; i < path_length; i++)
    {
        if (path[path_length - i] == '/')
        {
            l = i;
            break;
        }
    }
    char *directory = malloc(sizeof(char) * 100);
    for (int i = 0; i < path_length - l + 1; i++)
    {
        directory[i] = path[i];
    }
    directory[path_length - l + 1] = '\0';
    strcat(directory, file_name);
    return directory;
}

void model_3D_create_from_file(model_3D_t *model, const char *file)
{
    cgltf_options gltf_options = {0};
    cgltf_data *gltf_data = NULL;
    cgltf_result gltf_result = cgltf_parse_file(&gltf_options, file, &gltf_data);

    if (gltf_result != cgltf_result_success)
    {
        printf(LOG_ERROR "[GLTF]: Failed to parse |%s|\n", file);
        exit(-1);
    }
    else
    {
        cgltf_load_buffers(&gltf_options, gltf_data, file);
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

        model->material_count = gltf_data->materials_count;
        //printf(LOG_GLTF "Material count: %zu\n", model->material_count);
        model->materials = malloc(model->material_count * sizeof(material_t));

        texture_t empty_color_map;
        unsigned char empty_color_data[] = {128, 128, 255, 255};
        texture_create_from_data(&empty_color_map, empty_color_data, (vec2_t){1, 1}, false);

        texture_t empty_orm;
        unsigned char empty_orm_data[] = {255, 255, 0, 255};
        texture_create_from_data(&empty_orm, empty_orm_data, (vec2_t){1, 1}, false);

        for (int mat = 0; mat < model->material_count; mat++)
        {
            model->materials[mat].name = strcat(gltf_data->materials[mat].name, "");
            //printf(LOG_GLTF "Material: %s\n", model->materials[mat].name);

            // Base color
            if (gltf_data->materials[mat].pbr_metallic_roughness.base_color_texture.texture != NULL)
            {
                char *path = get_full_path(file, gltf_data->materials[mat].pbr_metallic_roughness.base_color_texture.texture->image->uri);
                texture_create_from_file(&model->materials[mat].diffuse_map, path, true, true);
                free(path);
            }
            else
            {
                model->materials[mat].diffuse_map = empty_color_map;
                printf(LOG_WARNING "[GLTF]: No base color texture in material!\n");
            }

            // Orm map
            if (gltf_data->materials[mat].pbr_metallic_roughness.metallic_roughness_texture.texture != NULL)
            {
                char *path = get_full_path(file, gltf_data->materials[mat].pbr_metallic_roughness.metallic_roughness_texture.texture->image->uri);
                texture_create_from_file(&model->materials[mat].orm_map, path, true, true);
                free(path);
            }
            else
            {
                model->materials[mat].orm_map = empty_orm;
            }

            // Normal map
            if (gltf_data->materials[mat].normal_texture.texture != NULL)
            {
                char *path = get_full_path(file, gltf_data->materials[mat].normal_texture.texture->image->uri);
                texture_create_from_file(&model->materials[mat].normal_map, path, true, false);
                free(path);
            }
            else
            {
                model->materials[mat].normal_map = empty_color_map;
            }
        }

        model->mesh_count = gltf_data->meshes_count;
        model->meshes = malloc(sizeof(mesh_t) * model->mesh_count);

        //printf(LOG_GLTF "Mesh Count: %u\n", model->mesh_count);

        for (int m = 0; m < model->mesh_count; m++)
        {
            model->meshes[m].primitive_count = gltf_data->meshes[m].primitives_count;
            model->meshes[m].primitives = malloc(model->meshes[m].primitive_count * sizeof(primitive_t));

            //printf(LOG_GLTF "Prime count: %u\n", model->meshes[m].primitive_count);
            //printf(LOG_GLTF "Mesh name: %s\n", gltf_data->meshes[m].name);

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

                // TO-DO
                // indices segmentaion fault

                model->meshes[m].primitives[p].index_count = gltf_data->meshes[m].primitives[p].indices->count;
                //printf(LOG_GLTF "Index count: %u\n", model->meshes[m].primitives[p].index_count);

                void *buffer = gltf_data->meshes[m].primitives[p].attributes->data->buffer_view->buffer->data;
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

                //printf(LOG_GLTF "    buffer %p\n", buffer);
                //printf(LOG_GLTF "    vertices %p\n", vertices);
                //printf(LOG_GLTF "    vertices size %zu\n", vertices_size);
                //printf(LOG_GLTF "    vertices offset %zu\n", vertices_offset);

                //printf(LOG_GLTF "    indices %p\n", indices);
                //printf(LOG_GLTF "    indices size %zu\n", indices_size);
                //printf(LOG_GLTF "    indices offset %zu\n", indices_offset);

                vertex_array_create(&model->meshes[m].primitives[p].vertex_array);
                vertex_array_create_vbo(&model->meshes[m].primitives[p].vertex_array, vertices, vertices_size, false);

                for (int atr = 0; atr < gltf_data->meshes[m].primitives[p].attributes_count; atr++)
                {
                    size_t atr_offset = gltf_data->meshes[m].primitives[p].attributes[atr].data->buffer_view->offset - vertices_offset;
                    size_t atr_stride = gltf_data->meshes[m].primitives[p].attributes[atr].data->buffer_view->stride;

                    char *atr_name = gltf_data->meshes[m].primitives[p].attributes[atr].name;
                    if (gltf_data->meshes[m].primitives[p].attributes[atr].data->component_type != cgltf_component_type_r_32f)
                    {
                        printf(LOG_WARNING "[GLTF]]: Attribute is not of type float!\n");
                    }
                
                    if (strcmp(atr_name, "POSITION") == 0)
                    {
                        //printf(LOG_GLTF "POSITION: offset: %zu, stride %zu\n", atr_offset, atr_stride);
                        vertex_array_push_attribute(0, 3, atr_stride, atr_offset);
                    }
                    else if (strcmp(atr_name, "NORMAL") == 0)
                    {
                        //printf(LOG_GLTF "NORMAL: offset: %zu, stride %zu\n", atr_offset, atr_stride);
                        vertex_array_push_attribute(1, 3, atr_stride, atr_offset);
                    }
                    else if (strcmp(atr_name, "TEXCOORD_0") == 0)
                    {
                        //printf(LOG_GLTF "TEXCOORD_0: offset: %zu, stride %zu\n", atr_offset, atr_stride);
                        vertex_array_push_attribute(2, 2, atr_stride, atr_offset);
                    }
                    else if (strcmp(atr_name, "TANGENT") == 0)
                    {
                        //printf(LOG_GLTF "TANGENT: offset: %zu, stride %zu\n", atr_offset, atr_stride);
                        vertex_array_push_attribute(3, 4, atr_stride, atr_offset);
                    }
                }
                vertex_array_create_ibo(&model->meshes[m].primitives[p].vertex_array, indices, indices_size, false);
                vertex_array_unbind();
                vertex_array_unbind_buffers();
            }
        }
    }
}
void model_3D_delete(model_3D_t *model)
{
    for (int m = 0; m < model->mesh_count; m++)
    {
        free(model->meshes[m].primitives);
    }
    for (int m = 0; m < model->material_count; m++)
    {
        texture_delete(&model->materials->diffuse_map);
        texture_delete(&model->materials->orm_map);
        texture_delete(&model->materials->normal_map);
    }

    free(model->meshes);
    free(model->materials);
}
