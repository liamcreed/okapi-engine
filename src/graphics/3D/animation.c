#include "graphics/graphics.h"

mesh_joint_t* mesh_joint_from_name(mesh_armature_t* armature, const char* name)
{
    for (i32 j = 0;j < armature->joint_count; j++)
    {
        if (strncmp(armature->joints[j].name, name, strlen(name) - 1) == 0)
        {
            return &armature->joints[j];
        }
    }

    printf(LOG_ERROR"[ANIMATOR]: joint with name [%s] not found!\n", name);
    exit(-1);
}

void armature_update_transforms(mesh_armature_t* armature)
{
    for (i32 j = 0; j < armature->joint_count; j++)
    {
        mesh_joint_t* joint = &armature->joints[j];

        mat4_t local_transform = mat4_rotate_q(mat4_translate(mat4_new(1), joint->location), joint->rotation);
        mat4_t global_matrix = local_transform;

        mesh_joint_t* next_joint = joint;

        while (next_joint->parent_id != -1)
        {
            mesh_joint_t* parent_joint = &armature->joints[next_joint->parent_id];

            mat4_t parent_local_transform = mat4_rotate_q(mat4_translate(mat4_new(1), parent_joint->location), parent_joint->rotation);
            global_matrix = mat4_multiply(parent_local_transform, global_matrix);

            next_joint = parent_joint;
        }

        armature->joint_matrices[j] = mat4_multiply(global_matrix, joint->inverse_bind_matrix);;
    }
}


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


void animation_play(mesh_armature_t* armature, mesh_animation_t* animation, f32 dt, f32 speed, bool loop)
{
    armature->animation = animation;

    u32 keyframe = armature->time * animation->frame_rate;

    for (i32 j = 0; j < armature->joint_count; j++)
    {
        mesh_joint_t* joint = &armature->joints[j];

        if (keyframe > animation->total_keyframe_count - 1)
        {
            armature->time = 0;
            keyframe = 0;
        }
        u32 next_frame;

        if (keyframe >= armature->animation->total_keyframe_count - 1)
        {
            next_frame = 0;
        }
        else
            next_frame = keyframe + 1;

        float s = dt * armature->animation->frame_rate * speed / 2;
        joint->rotation = quat_lerp(joint->rotation, armature->animation->key_frames[j][next_frame].rotation, s);
        joint->location = vec3_lerp(joint->location, armature->animation->key_frames[j][next_frame].location, s);
    }

    armature->time += dt * speed;

}