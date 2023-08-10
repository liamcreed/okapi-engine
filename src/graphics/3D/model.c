#include "graphics/graphics.h"

#define CGLTF_IMPLEMENTATION
#include "cgltf/cgltf.h"

void skeleton_build_joint_hierarchy(mesh_joint_t* root, mesh_armature_t* armature, cgltf_skin* skin, cgltf_node* node)
{
    printf("-------------------\n");

    strncpy(root->name, node->name, sizeof(root->name) -1);
    root->name[sizeof(root->name) - 1] = '\0';
    printf("NAME: %s\n", root->name);

    root->child_count = node->children_count;
    printf("CHILD COUNT: %u\n", root->child_count);

    for (i32 i = 0; i < skin->joints_count; i++)
    {
        if(node == skin->joints[i])
            root->id = i;
    }
    armature->joints[root->id] = root;

    printf("ID %u\n", root->id); 

    f32 inverse_bind_matrix[16];
    cgltf_accessor_read_float(skin->inverse_bind_matrices, root->id,inverse_bind_matrix, 16);
    root->inverse_bind_transform = mat4_1D_to_2D(inverse_bind_matrix);
    mat4_print(root->inverse_bind_transform);

    root->anim_transform = mat4_new(1);

    if (root->child_count > 0)
    {
        root->children = malloc(sizeof(mesh_joint_t) * root->child_count);
        for (u32 i = 0; i < root->child_count; i++)
        {
            root->children[i].parent = root;
            skeleton_build_joint_hierarchy(&root->children[i],armature ,skin,node->children[i]);
            
        }
    }
    
}


void model_3D_create(model_3D_t* model)
{
    for (i32 m = 0; m < model->mesh_count; m++)
    {
        for (i32 p = 0; p < model->meshes[m].primitive_count; p++)
        {
            vertex_array_create(&model->meshes[m].primitives[p].vertex_array);
            vertex_array_create_vbo(&model->meshes[m].primitives[p].vertex_array, model->meshes[m].primitives[p].vertices, model->meshes[m].primitives[p].vertices_size, false);

            model->meshes[m].primitives[p].attribute_count = model->meshes[m].primitives[p].attribute_count;

            for (i32 atr = 0; atr < model->meshes[m].primitives[p].attribute_count; atr++)
            {
                if (atr > MAX_ATTRIBUTE_COUNT)
                {
                    printf(LOG_WARNING"[MODEL 3D]: exceeding maximum attributes for %s!\n", model->meshes[m].name);
                }
                size_t atr_offset = model->meshes[m].primitives[p].attributes[atr].offset;
                size_t atr_stride = model->meshes[m].primitives[p].attributes[atr].stride;

                if (strcmp(model->meshes[m].primitives[p].attributes[atr].name, "POSITION") == 0)
                {
                    vertex_array_push_attribute_f(0, 3, atr_stride, atr_offset);
                }
                else if (strcmp(model->meshes[m].primitives[p].attributes[atr].name, "NORMAL") == 0)
                {
                    vertex_array_push_attribute_f(1, 3, atr_stride, atr_offset);
                }
                else if (strcmp(model->meshes[m].primitives[p].attributes[atr].name, "TEXCOORD_0") == 0)
                {
                    vertex_array_push_attribute_f(2, 2, atr_stride, atr_offset);
                }
                else if (strcmp(model->meshes[m].primitives[p].attributes[atr].name, "TANGENT") == 0)
                {
                    vertex_array_push_attribute_f(3, 4, atr_stride, atr_offset);
                }
                else if (strcmp(model->meshes[m].primitives[p].attributes[atr].name, "JOINTS_0") == 0)
                {
                    vertex_array_push_attribute_ub(4, 4, atr_stride, atr_offset);
                }
                else if (strcmp(model->meshes[m].primitives[p].attributes[atr].name, "WEIGHTS_0") == 0)
                {
                    vertex_array_push_attribute_f(5, 4, atr_stride, atr_offset);
                }
            }
            vertex_array_create_ibo(&model->meshes[m].primitives[p].vertex_array, model->meshes[m].primitives[p].indices, model->meshes[m].primitives[p].indices_size, false);
            vertex_array_unbind();
            vertex_array_unbind_buffers();
        }
    }
}

void model_3D_load_from_GLTF(model_3D_t* model, const char* path)
{
    cgltf_options gltf_options = { 0 };
    cgltf_data* gltf_data = NULL;
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

    u8 empty_color_data[] = { 128, 128, 255, 255 };
    texture_t empty_color_map =
    {
        .data = empty_color_data,
        .channel_count = 4,
        .width = 1,
        .height = 1,
        .sRGB = true
    };
    texture_create(&empty_color_map);

    u8 empty_orm_data[] = { 255, 255, 0, 255 };
    texture_t empty_orm_map = 
    {
        .data = empty_orm_data,
        .channel_count = 4,
        .width = 1,
        .height = 1,
        .sRGB = true
    };
    texture_create(&empty_orm_map);

    model->material_count = gltf_data->materials_count;
    for (i32 mat = 0; mat < model->material_count; mat++)
    {
        strncpy(model->materials[mat].name, gltf_data->materials[mat].name, sizeof(model->materials[mat].name) - 1);
        model->materials[mat].name[sizeof(model->materials[mat].name) - 1] = '\0';

        if (gltf_data->materials[mat].pbr_metallic_roughness.base_color_texture.texture != NULL)
        {
            char* image_path = get_full_path_from_other(path, gltf_data->materials[mat].pbr_metallic_roughness.base_color_texture.texture->image->uri);
            model->materials[mat].diffuse_map.sRGB = true;
            texture_load_from_TGA(&model->materials[mat].diffuse_map, image_path);
            texture_create(&model->materials[mat].diffuse_map);
            free(image_path);
        }
        else
        {
            model->materials[mat].diffuse_map = empty_color_map;
            printf(LOG_WARNING "[GLTF]: No base color texture in material!\n");
        }

        model->materials[mat].orm_map = empty_orm_map;
        model->materials[mat].normal_map = empty_color_map;
    }

    model->animation_count = gltf_data->animations_count;
    for (i32 a = 0; a < model->animation_count; a++)
    {
        strncpy(model->animations[a].name, gltf_data->animations[a].name, sizeof(model->animations[a].name) - 1);
        model->animations[a].name[sizeof(model->animations[a].name)-1] = '\0';
    }
    if(model->animation_count)
    {
        cgltf_node* root_node = gltf_data->skins[0].joints[0];
        model->armature.joint_count = gltf_data->skins[0].joints_count;
        skeleton_build_joint_hierarchy(&model->armature.root_joint, &model->armature,gltf_data->skins, root_node);

    }
    

    model->mesh_count = gltf_data->meshes_count;
    for (i32 m = 0; m < model->mesh_count; m++)
    {
        strncpy(model->meshes[m].name, gltf_data->meshes[m].name, sizeof(model->meshes[m].name) - 1);
        model->meshes[m].name[sizeof(model->meshes[m].name) - 1] = '\0';

        model->meshes[m].primitive_count = gltf_data->meshes[m].primitives_count;
        for (i32 p = 0; p < model->meshes[m].primitive_count; p++)
        {
            bool material_found;
            for (i32 mat = 0; mat < model->material_count; mat++)
            {
                if (strcmp(gltf_data->meshes[m].primitives[p].material->name, model->materials[mat].name) == 0)
                {
                    model->meshes[m].primitives[p].material_index = mat;
                    material_found = true;
                }
            }
            if (!material_found)
            {
                printf(LOG_ERROR "[GLTF]:NO MATERIAL FOUND!\n");
            }

            void* buffer = gltf_data->meshes[m].primitives[p].attributes->data->buffer_view->buffer->data;

            size_t vertices_offset = gltf_data->meshes[m].primitives[p].attributes->data->buffer_view->offset;
            size_t indices_offset = gltf_data->meshes[m].primitives[p].indices->buffer_view->offset;

            model->meshes[m].primitives[p].vertices_size = indices_offset - vertices_offset;
            model->meshes[m].primitives[p].vertices = malloc(model->meshes[m].primitives[p].vertices_size);
            memcpy(model->meshes[m].primitives[p].vertices, buffer + vertices_offset, model->meshes[m].primitives[p].vertices_size);

            model->meshes[m].primitives[p].index_count = gltf_data->meshes[m].primitives[p].indices->count;
            model->meshes[m].primitives[p].indices_size = gltf_data->meshes[m].primitives[p].indices->buffer_view->size;
            model->meshes[m].primitives[p].indices = malloc(model->meshes[m].primitives[p].indices_size);
            memcpy(model->meshes[m].primitives[p].indices, buffer + indices_offset, model->meshes[m].primitives[p].indices_size);

            u32 index_type = gltf_data->meshes[m].primitives[p].indices->component_type;
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

            model->meshes[m].primitives[p].attribute_count = gltf_data->meshes[m].primitives[p].attributes_count;

            for (i32 atr = 0; atr < model->meshes[m].primitives[p].attribute_count; atr++)
            {
                model->meshes[m].primitives[p].attributes[atr].offset = gltf_data->meshes[m].primitives[p].attributes[atr].data->buffer_view->offset - vertices_offset;
                model->meshes[m].primitives[p].attributes[atr].stride = gltf_data->meshes[m].primitives[p].attributes[atr].data->buffer_view->stride;
                strncpy(model->meshes[m].primitives[p].attributes[atr].name, gltf_data->meshes[m].primitives[p].attributes[atr].name, sizeof(model->meshes[m].primitives[p].attributes[atr].name) - 1);
                model->meshes[m].name[sizeof(model->meshes[m].primitives[p].attributes[atr].name) - 1] = '\0';
            }
        }
    }
    free(gltf_data);


    FILE* file;
    file = fopen("./res/3D/knight.okp3D", "wb");
    fwrite(&model->mesh_count, sizeof(u32), 1, file);
    for (i32 m = 0; m < model->mesh_count; m++)
    {
        fwrite(model->meshes[m].name, 1, 32, file);
        fwrite(&model->meshes[m].primitive_count, sizeof(u32), 1, file);

    }

    fclose(file);


    model_3D_create(model);
    printf("created model\n");
}

void model_3D_delete(model_3D_t* model)
{
    for (i32 m = 0; m < model->material_count; m++)
    {
        texture_delete(&model->materials->diffuse_map);
        texture_delete(&model->materials->orm_map);
        texture_delete(&model->materials->normal_map);
    }
    for (i32 m = 0; m < model->mesh_count; m++)
    {
        for (i32 p = 0; p < model->meshes[m].primitive_count; p++)
        {
            free(model->meshes[m].primitives[p].vertices);
            free(model->meshes[m].primitives[p].indices);
        }
    }
}
