#include "graphics/graphics.h"


mesh_animation_t* mesh_anim_from_name(model_3D_t* model, const char* name)
{
    for (i32 a = 0; a < model->animation_count; a++)
    {
        if (strncmp(model->animations[a].name, name, strlen(name) - 1) == 0)
        {
            return &model->animations[a];
        }
    }

    printf(LOG_ERROR"[ANIMATOR]: animation with name [%s] not found!\n", name);
    exit(-1);
}