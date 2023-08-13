#include "graphics/graphics.h"

void model_3D_create(model_3D_t* model)
{
    for (i32 m = 0; m < model->mesh_count; m++)
    {
        for (i32 p = 0; p < model->meshes[m].primitive_count; p++)
        {
            vertex_array_create(&model->meshes[m].primitives[p].vertex_array);
            vertex_array_create_vbo(&model->meshes[m].primitives[p].vertex_array, model->meshes[m].primitives[p].vertices, model->meshes[m].primitives[p].vertices_size, false);

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
    for (i32 a = 0; a < model->animation_count; a++)
    {
        free(model->animations[a].key_frames);
    }
    
}
