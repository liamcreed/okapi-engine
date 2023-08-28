#include "graphics/graphics.h"

void model_3D_create(model_3D_t* model)
{
    for (i32 mat = 0; mat < model->material_count; mat++)
    {
        model->materials[mat].diffuse_map.sRGB = true;
        model->materials[mat].diffuse_map.filter = true;
        texture_create(&model->materials[mat].diffuse_map);
        texture_create(&model->materials[mat].orm_map);
        texture_create(&model->materials[mat].normal_map);
    }

    for (i32 a = 0; a < model->animation_count; a++)
    {
        model->animations[a].frame_rate = model->animations[a].total_keyframe_count / model->animations[a].duration;
    }

    model->armature.joint_matrices = malloc(sizeof(mat4) * model->armature.joint_count);

    model->armature.time = 0;
    model->armature.playing_anim = false;

    for (i32 m = 0; m < model->mesh_count; m++)
    {
        for (i32 p = 0; p < model->meshes[m].primitive_count; p++)
        {
            vertex_array_create(&model->meshes[m].primitives[p].vertex_array);
            vertex_array_create_vbo(&model->meshes[m].primitives[p].vertex_array, model->meshes[m].primitives[p].vertices, model->meshes[m].primitives[p].vertices_size, false);

            for (i32 atr = 0; atr < model->meshes[m].primitives[p].attribute_count; atr++)
            {
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

    for (i32 m = 0; m < model->mesh_count; m++)
    {
        for (i32 p = 0; p < model->meshes[m].primitive_count; p++)
        {
            free(model->meshes[m].primitives[p].vertices);
            free(model->meshes[m].primitives[p].indices);
        }
    }
}

void model_3D_delete(model_3D_t* model)
{
    for (i32 m = 0; m < model->material_count; m++)
    {
        texture_delete(&model->materials->diffuse_map);
        texture_delete(&model->materials->orm_map);
        texture_delete(&model->materials->normal_map);

    }
    free(model->materials);

    free(model->meshes);

    for (i32 a = 0; a < model->animation_count; a++)
    {
        free(model->animations[a].key_frames);
    }
    free(model->animations);
}

void model_3D_export_to_bin(model_3D_t* model, const char* path)
{
    FILE* file = fopen(path, "wb");
    if (file == NULL)
        printf(LOG_ERROR"[MODEL 3D]: failed to create file %s\n", path);

    fwrite(&model->material_count, sizeof(model->material_count), 1, file);

    for (i32 i = 0; i < model->material_count; i++)
    {
        mesh_material_t* material = &model->materials[i];
        fwrite(material->name, 32, 1, file);

        fwrite(&material->diffuse_map.channel_count, sizeof(u32), 1, file);
        fwrite(&material->diffuse_map.width, sizeof(u32), 1, file);
        fwrite(&material->diffuse_map.height, sizeof(u32), 1, file);

        fwrite(material->diffuse_map.data, material->diffuse_map.width * material->diffuse_map.height * material->diffuse_map.channel_count, 1, file);
        fwrite(&material->color, sizeof(vec4), 1, file);

        fwrite(&material->orm_map.channel_count, sizeof(u32), 1, file);
        fwrite(&material->orm_map.width, sizeof(u32), 1, file);
        fwrite(&material->orm_map.height, sizeof(u32), 1, file);

        fwrite(material->orm_map.data, material->orm_map.width * material->orm_map.height * material->orm_map.channel_count, 1, file);

        fwrite(&material->normal_map.channel_count, sizeof(u32), 1, file);
        fwrite(&material->normal_map.width, sizeof(u32), 1, file);
        fwrite(&material->normal_map.height, sizeof(u32), 1, file);

        fwrite(material->normal_map.data, material->normal_map.width * material->normal_map.height * material->normal_map.channel_count, 1, file);
    }

    fwrite(&model->mesh_count, sizeof(model->mesh_count), 1, file);

    for (i32 i = 0; i < model->mesh_count; i++)
    {
        mesh_t* mesh = &model->meshes[i];
        fwrite(mesh->name, 32, 1, file);
        fwrite(&mesh->primitive_count, sizeof(mesh->primitive_count), 1, file);

        for (i32 p = 0; p < mesh->primitive_count; p++)
        {
            mesh_primitive_t* primitive = &mesh->primitives[p];
            fwrite(&primitive->material_index, sizeof(u32), 1, file);

            fwrite(&primitive->vertices_size, sizeof(size_t), 1, file);
            fwrite(primitive->vertices, primitive->vertices_size, 1, file);

            fwrite(&primitive->indices_size, sizeof(primitive->indices_size), 1, file);
            fwrite(&primitive->index_type, sizeof(primitive->index_type), 1, file);
            fwrite(&primitive->index_count, sizeof(primitive->index_count), 1, file);
            fwrite(primitive->indices, primitive->indices_size, 1, file);

            fwrite(&primitive->attribute_count, sizeof(u32), 1, file);

            if (primitive->attribute_count > MAX_VERTEX_ATTRIBUTE_COUNT)
                exit(-1);
            for (i32 a = 0; a < primitive->attribute_count; a++)
            {
                fwrite(&primitive->attributes[a], sizeof(mesh_vertex_attribute_t), 1, file);
            }
        }
    }
    fwrite(&model->armature.joint_count, sizeof(u32), 1, file);
    for (i32 i = 0; i < model->armature.joint_count; i++)
    {
        fwrite(&model->armature.joints[i], sizeof(mesh_joint_t), 1, file);
    }


    fwrite(&model->animation_count, sizeof(u32), 1, file);
    for (i32 a = 0; a < model->animation_count; a++)
    {
        mesh_animation_t* animation = &model->animations[a];
        fwrite(animation->name, 64, 1, file);
        fwrite(&animation->duration, sizeof(f32), 1, file);
        fwrite(&animation->total_keyframe_count, sizeof(u32), 1, file);

        for (i32 j = 0; j < model->armature.joint_count; j++)
        {
            fwrite(&animation->key_frame_count[j], sizeof(u32), 1, file);
            for (i32 k = 0; k < MAX_KEY_FRAME_COUNT; k++)
            {
                fwrite(&animation->key_frames[j][k].time_stamp, sizeof(f32), 1, file);
                fwrite(&animation->key_frames[j][k].location, sizeof(vec3), 1, file);
                fwrite(&animation->key_frames[j][k].rotation, sizeof(vec4), 1, file);
            }
        }
    }

    fclose(file);
}

void model_3D_load_from_bin(model_3D_t* model, const char* path)
{
    *model = (model_3D_t){};
    FILE* file = fopen(path, "rb");
    if (file == NULL)
        printf(LOG_ERROR"[MODEL 3D]: failed to open file %s\n", path);

    fread(&model->material_count, sizeof(model->material_count), 1, file);
    model->materials = malloc(sizeof(mesh_material_t) * model->material_count);

    for (i32 i = 0; i < model->material_count; i++)
    {
        model->materials[i] = (mesh_material_t){};
        mesh_material_t* material = &model->materials[i];

        fread(material->name, 32, 1, file);

        fread(&material->diffuse_map.channel_count, sizeof(u32), 1, file);
        fread(&material->diffuse_map.width, sizeof(u32), 1, file);
        fread(&material->diffuse_map.height, sizeof(u32), 1, file);
        material->diffuse_map.data = malloc(material->diffuse_map.width * material->diffuse_map.height * material->diffuse_map.channel_count);
        fread(material->diffuse_map.data, material->diffuse_map.width * material->diffuse_map.height * material->diffuse_map.channel_count, 1, file);

        fread(&material->color, sizeof(vec4), 1, file);

        fread(&material->orm_map.channel_count, sizeof(u32), 1, file);
        fread(&material->orm_map.width, sizeof(u32), 1, file);
        fread(&material->orm_map.height, sizeof(u32), 1, file);
        material->orm_map.data = malloc(material->orm_map.width * material->orm_map.height * material->orm_map.channel_count);
        fread(material->orm_map.data, material->orm_map.width * material->orm_map.height * material->orm_map.channel_count, 1, file);

        fread(&material->normal_map.channel_count, sizeof(u32), 1, file);
        fread(&material->normal_map.width, sizeof(u32), 1, file);
        fread(&material->normal_map.height, sizeof(u32), 1, file);
        material->normal_map.data = malloc(material->orm_map.width * material->orm_map.height * material->orm_map.channel_count);
        fread(material->normal_map.data, material->normal_map.width * material->normal_map.height * material->normal_map.channel_count, 1, file);
    }

    fread(&model->mesh_count, sizeof(model->mesh_count), 1, file);
    model->meshes = malloc(sizeof(mesh_t) * model->mesh_count);

    for (i32 i = 0; i < model->mesh_count; i++)
    {
        model->meshes[i] = (mesh_t){};
        mesh_t* mesh = &model->meshes[i];

        fread(mesh->name, 32, 1, file);

        fread(&mesh->primitive_count, sizeof(mesh->primitive_count), 1, file);
        mesh->primitives = malloc(sizeof(mesh_primitive_t) * mesh->primitive_count);

        for (i32 p = 0; p < mesh->primitive_count; p++)
        {
            mesh->primitives[p] = (mesh_primitive_t){};
            mesh_primitive_t* primitive = &mesh->primitives[p];

            fread(&primitive->material_index, sizeof(u32), 1, file);

            fread(&primitive->vertices_size, sizeof(size_t), 1, file);

            primitive->vertices = malloc(primitive->vertices_size);
            fread(primitive->vertices, primitive->vertices_size, 1, file);

            fread(&primitive->indices_size, sizeof(primitive->indices_size), 1, file);

            fread(&primitive->index_type, sizeof(primitive->index_type), 1, file);

            fread(&primitive->index_count, sizeof(primitive->index_count), 1, file);

            primitive->indices = malloc(primitive->indices_size);
            fread(primitive->indices, primitive->indices_size, 1, file);

            fread(&primitive->attribute_count, sizeof(u32), 1, file);

            for (i32 a = 0; a < primitive->attribute_count; a++)
            {
                fread(&primitive->attributes[a], sizeof(mesh_vertex_attribute_t), 1, file);
            }
        }
    }

    model->armature = (mesh_armature_t){};

    fread(&model->armature.joint_count, sizeof(u32), 1, file);
    model->armature.joints = malloc(sizeof(mesh_joint_t) * model->armature.joint_count);

    for (i32 i = 0; i < model->armature.joint_count; i++)
    {
        fread(&model->armature.joints[i], sizeof(mesh_joint_t), 1, file);
    }

    fread(&model->animation_count, sizeof(u32), 1, file);
    model->animations = malloc(sizeof(mesh_animation_t) * model->animation_count);

    for (i32 a = 0; a < model->animation_count; a++)
    {
        mesh_animation_t* animation = &model->animations[a];
        fread(animation->name, 64, 1, file);

        fread(&animation->duration, sizeof(f32), 1, file);
        fread(&animation->total_keyframe_count, sizeof(u32), 1, file);

        animation->key_frames = malloc(model->armature.joint_count * MAX_KEY_FRAME_COUNT * sizeof(key_frame_t));
        animation->key_frame_count = malloc(sizeof(u32) * model->armature.joint_count);

        for (i32 j = 0; j < model->armature.joint_count; j++)
        {
            fread(&animation->key_frame_count[j], sizeof(u32), 1, file);
            for (i32 k = 0; k < MAX_KEY_FRAME_COUNT; k++)
            {
                fread(&animation->key_frames[j][k].time_stamp, sizeof(f32), 1, file);
                fread(&animation->key_frames[j][k].location, sizeof(vec3), 1, file);
                fread(&animation->key_frames[j][k].rotation, sizeof(vec4), 1, file);
            }
        }
    }

    fclose(file);
}