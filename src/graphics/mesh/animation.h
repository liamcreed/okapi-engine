#pragma once


void armature_update_transforms(mesh_armature_t* armature);
void animation_play(mesh_armature_t* armature, mesh_animation_t* animation, f32 dt, f32 speed, bool loop);
typedef struct 
{
    f64 time;
}mesh_animator_t;

#define ANIM_FRAMERATE 24

mesh_animation_t* mesh_anim_from_name(model_3D_t* model, const char* name);