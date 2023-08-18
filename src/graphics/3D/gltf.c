#include "graphics/graphics.h"

#define CGLTF_IMPLEMENTATION
#include <cgltf/cgltf.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>


void texture_load_from_PNG(texture_t* texture, const char* path)
{
    stbi_set_flip_vertically_on_load(false);
    int channel_count;
    int x, y;
    texture->data = stbi_load(path, &x, &y, &channel_count, 0);

    texture->width = x;
    texture->height = y;
    texture->channel_count = channel_count;

    if (texture->data == NULL)
    {
        printf(LOG_ERROR"[texture]: failed to load texture: %s\n", path);
        exit(-1);
    }
}

void armature_build_joint_hierarchy(mesh_armature_t* armature, cgltf_skin* skin)
{
    armature->joint_count = skin->joints_count;
    armature->joints = malloc(sizeof(mesh_joint_t) * armature->joint_count);

    for (i32 i = 0; i < armature->joint_count; i++)
    {
        cgltf_node* current_node = skin->joints[i];

        armature->joints[i] = (mesh_joint_t){};
        armature->joints[i].id = i;

        strncpy(armature->joints[i].name, current_node->name, sizeof(armature->joints[i].name) - 1);
        armature->joints[i].name[sizeof(armature->joints[i].name) - 1] = '\0';

        armature->joints[i].parent_id = -1;

        for (i32 j = 0; j < armature->joint_count; j++)
        {
            if (strcmp(skin->joints[j]->name, skin->joints[i]->parent->name) == 0)
                armature->joints[i].parent_id = j;
        }

        f32 inverse_bind_matrix[16];
        cgltf_accessor_read_float(skin->inverse_bind_matrices, armature->joints[i].id, inverse_bind_matrix, 16);
        armature->joints[i].inverse_bind_matrix = mat4_1D_to_2D(inverse_bind_matrix);

        f32 trans_local[16];
        cgltf_node_transform_local(current_node, trans_local);
        armature->joints[i].local_bind_matrix = mat4_1D_to_2D(trans_local);

        armature->joints[i].location.x = skin->joints[i]->translation[0];
        armature->joints[i].location.y = skin->joints[i]->translation[1];
        armature->joints[i].location.z = skin->joints[i]->translation[2];

        armature->joints[i].rotation.x = skin->joints[i]->rotation[0];
        armature->joints[i].rotation.y = skin->joints[i]->rotation[1];
        armature->joints[i].rotation.z = skin->joints[i]->rotation[2];
        armature->joints[i].rotation.w = skin->joints[i]->rotation[3];
    }
}

char* get_full_path_from_other(const char* path, char* file_name)
{
    u32 path_length = strlen(path);
    u32 l;

    for (u32 i = 0; i < path_length; i++)
    {
        if (path[path_length - i] == '/')
        {
            l = i;
            break;
        }
    }
    char* directory = malloc(sizeof(char) * 100);
    for (u32 i = 0; i < path_length - l + 1; i++)
    {
        directory[i] = path[i];
    }
    directory[path_length - l + 1] = '\0';
    strcat(directory, file_name);

    return directory;
}

void model_3D_load_from_GLTF(model_3D_t* model, const char* path)
{
    *model = (model_3D_t){};

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

    //--------------------------------------------------//

    if (gltf_data->skins_count)
    {
        model->armature = (mesh_armature_t){};
        armature_build_joint_hierarchy(&model->armature, gltf_data->skins);

        model->animation_count = gltf_data->animations_count;
        model->animations = malloc(sizeof(mesh_animation_t) * model->animation_count);

        for (i32 a = 0; a < model->animation_count; a++)
        {
            model->animations[a] = (mesh_animation_t){};

            strncpy(model->animations[a].name, gltf_data->animations[a].name, sizeof(model->animations[a].name) - 1);
            model->animations[a].name[sizeof(model->animations[a].name) - 1] = '\0';

            model->animations[a].duration = 0;

            model->animations[a].key_frames = malloc(model->armature.joint_count * MAX_KEY_FRAME_COUNT * sizeof(key_frame_t));
            model->animations[a].key_frame_count = malloc(sizeof(u32) * model->armature.joint_count);
    
            for (u32 j = 0; j < model->armature.joint_count; j++)
            {
                model->animations[a].key_frame_count[j] = 0;

                for (i32 k = 0; k < MAX_KEY_FRAME_COUNT; k++)
                {
                    model->animations[a].key_frames[j][k].rotation = model->armature.joints[j].rotation;
                    model->animations[a].key_frames[j][k].location = model->armature.joints[j].location;
                    model->animations[a].key_frames[j][k].time_stamp = 0;
                }
            }

            u32 channel_count = gltf_data->animations[a].channels_count;

            float sampler_max = 0;
            for (i32 c = 0; c < channel_count; c++)
            {
                cgltf_animation_channel* current_anim_channel = &gltf_data->animations[a].channels[c];
                for (i32 s = 0; s < current_anim_channel->sampler->input->count; s++)
                {
                    if (current_anim_channel->sampler->input->max[s] > sampler_max)
                        sampler_max = current_anim_channel->sampler->input->max[s];
                }
            }
            model->animations[a].duration = sampler_max;

            for (i32 c = 0; c < channel_count; c++)
            {
                cgltf_animation_channel* current_anim_channel = &gltf_data->animations[a].channels[c];

                i32 current_anim_joint = -1;
                for (i32 j = 0; j < model->armature.joint_count; j++)
                {
                    if (current_anim_channel->target_node == gltf_data->skins->joints[j])
                        current_anim_joint = j;
                }

                cgltf_accessor* timestamp_accessor = current_anim_channel->sampler->input;
                cgltf_accessor* transform_accessor = current_anim_channel->sampler->output;

                for (i32 i = 0; i < timestamp_accessor->count; ++i)
                {
                    f32 time_stamp;
                    cgltf_accessor_read_float(timestamp_accessor, i, &time_stamp, 1);

                    i32 index = i;
                    if (timestamp_accessor->count < transform_accessor->count)
                        index = i * 3 + 1;

                    if (current_anim_channel->target_path == cgltf_animation_path_type_rotation)
                    {
                        model->animations[a].key_frame_count[current_anim_joint] += 1;

                        f32 rotation[4];
                        cgltf_accessor_read_float(transform_accessor, index, rotation, 4);

                        vec4_t joint_rotation;
                        joint_rotation.x = rotation[0];
                        joint_rotation.y = rotation[1];
                        joint_rotation.z = rotation[2];
                        joint_rotation.w = rotation[3];

                        model->animations[a].key_frames[current_anim_joint][i].rotation = joint_rotation;
                        model->animations[a].key_frames[current_anim_joint][i].time_stamp = time_stamp;
                    }
                    else if (current_anim_channel->target_path == cgltf_animation_path_type_translation)
                    {

                        f32 location[3];
                        cgltf_accessor_read_float(transform_accessor, index, location, 3);

                        vec3_t joint_location;
                        joint_location.x = location[0];
                        joint_location.y = location[1];
                        joint_location.z = location[2];

                        model->animations[a].key_frames[current_anim_joint][i].location = joint_location;
                    }
                }
            }
            
            u32 max_keyframes = 0;
            for (i32 j = 0; j < model->armature.joint_count; j++)
            {
                if (model->animations[a].key_frame_count[j] > MAX_KEY_FRAME_COUNT)
                    printf(LOG_WARNING"[GLTF]: MAX KEYFRAMES REACHED! %u\n", model->animations[a].key_frame_count[j]);

                if (model->animations[a].key_frame_count[j] > max_keyframes)
                    max_keyframes = model->animations[a].key_frame_count[j];
            }
            model->animations[a].total_keyframe_count = max_keyframes;
        }
    }

    u8* empty_color_data = malloc(4);
    empty_color_data[0] = 255;
    empty_color_data[1] = 255;
    empty_color_data[2] = 255;
    empty_color_data[3] = 255;
    texture_t empty_color_map =
    {
        .data = empty_color_data,
        .channel_count = 4,
        .width = 1,
        .height = 1,
        .sRGB = true
    };
    texture_create(&empty_color_map);

    u8* empty_orm_data = malloc(4);
    empty_orm_data[0] = 255;
    empty_orm_data[1] = 255;
    empty_orm_data[2] = 0;
    empty_orm_data[3] = 255;
    
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
    model->materials = malloc(sizeof(mesh_material_t) * model->material_count);

    for (i32 mat = 0; mat < model->material_count; mat++)
    {
        model->materials[mat] = (mesh_material_t){};

        strncpy(model->materials[mat].name, gltf_data->materials[mat].name, sizeof(model->materials[mat].name) - 1);
        model->materials[mat].name[sizeof(model->materials[mat].name) - 1] = '\0';

        if (gltf_data->materials[mat].pbr_metallic_roughness.base_color_texture.texture != NULL)
        {
            char* image_path = get_full_path_from_other(path, gltf_data->materials[mat].pbr_metallic_roughness.base_color_texture.texture->image->uri);
            
            texture_load_from_PNG(&model->materials[mat].diffuse_map, image_path);
            free(image_path);
        }
        else
        {
            model->materials[mat].diffuse_map = empty_color_map;
        }

        if(gltf_data->materials[mat].pbr_metallic_roughness.base_color_factor)
        {
            float* color = gltf_data->materials[mat].pbr_metallic_roughness.base_color_factor;
            model->materials[mat].color.x = color[0];
            model->materials[mat].color.y = color[1];
            model->materials[mat].color.z = color[2];
            model->materials[mat].color.w = color[3];
        }
        else
            model->materials[mat].color = (vec4_t){1,1,1,1};


        model->materials[mat].orm_map = empty_orm_map;
        model->materials[mat].normal_map = empty_color_map;
    }

    model->mesh_count = gltf_data->meshes_count;
    model->meshes = malloc(sizeof(mesh_t) * model->mesh_count);

    for (i32 m = 0; m < model->mesh_count; m++)
    {
        model->meshes[m] = (mesh_t){};

        strncpy(model->meshes[m].name, gltf_data->meshes[m].name, sizeof(model->meshes[m].name) - 1);
        model->meshes[m].name[sizeof(model->meshes[m].name) - 1] = '\0';

        model->meshes[m].primitive_count = gltf_data->meshes[m].primitives_count;
        model->meshes[m].primitives = malloc(sizeof(mesh_primitive_t) * model->meshes[m].primitive_count);
        
        for (i32 p = 0; p < model->meshes[m].primitive_count; p++)
        {
            model->meshes[m].primitives[p] = (mesh_primitive_t){};

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
            if (model->meshes[m].primitives[p].attribute_count > MAX_VERTEX_ATTRIBUTE_COUNT)
                printf(LOG_WARNING"[GLTF]: MAX ATTRIBUTES REACHED! %u\n", model->meshes[m].primitives[p].attribute_count);

            for (i32 atr = 0; atr < model->meshes[m].primitives[p].attribute_count; atr++)
            {

                model->meshes[m].primitives[p].attributes[atr] = (mesh_vertex_attribute_t){};

                model->meshes[m].primitives[p].attributes[atr].offset = gltf_data->meshes[m].primitives[p].attributes[atr].data->buffer_view->offset - vertices_offset;
                model->meshes[m].primitives[p].attributes[atr].stride = gltf_data->meshes[m].primitives[p].attributes[atr].data->buffer_view->stride;
                strncpy(model->meshes[m].primitives[p].attributes[atr].name, gltf_data->meshes[m].primitives[p].attributes[atr].name, sizeof(model->meshes[m].primitives[p].attributes[atr].name) - 1);
                model->meshes[m].name[sizeof(model->meshes[m].primitives[p].attributes[atr].name) - 1] = '\0';
            }
        }
    }
   

    free(gltf_data);
}
