#pragma once

typedef struct 
{
    f32 time;
}mesh_animator_t;

#define ANIM_FRAMERATE 24

mesh_animation_t* mesh_anim_from_name(model_3D_t* model, const char* name);