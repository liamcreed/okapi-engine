#pragma once

typedef struct mesh_joint_t mesh_joint_t;
struct mesh_joint_t
{
    u32 id;
    char *name;
    mesh_joint_t *children;
    u32 child_count;
    mesh_joint_t *parent;
    mat4_t inverse_bind_transform;
};

typedef struct
{
    vec3_t scale;
    f32 time;
} key_frame_scale_t;

typedef struct
{
    vec4_t rotation;
    f32 time;
} key_frame_rot_t;

typedef struct
{
    vec3_t position;
    f32 time;
} key_frame_trans_t;

#define MAX_KEY_FRAME_COUNT 1
typedef struct
{
    const char *name;
    key_frame_scale_t key_frame_scale[MAX_KEY_FRAME_COUNT];
    key_frame_trans_t key_frame_trans[MAX_KEY_FRAME_COUNT];
    key_frame_rot_t key_frame_rot[MAX_KEY_FRAME_COUNT];
} mesh_animation_t;

typedef struct
{
    const char *name;
    mesh_joint_t root_joint;
    u32 joint_count;
} mesh_armature_t;

void mesh_animation_play(mesh_armature_t *armature, mesh_animation_t *animation, f32 dt);
 
//-------------------------------------------------------------//

typedef struct
{
    texture_t diffuse_map;
    texture_t orm_map;
    texture_t normal_map;
    const char *name;
} mesh_material_t;

typedef struct 
{
    size_t size;
    size_t offset;
    size_t stride;
}mesh_vertex_attribute_t;

#define MAX_ATTRIBUTE_COUNT 7
typedef struct
{
    mesh_material_t *material;
    vertex_array_t vertex_array;

    void* vertices;
    u32 vertex_size;
    
    u32* indices;
    u32 index_count;
    u32 index_type;

    u32 attribute_count;
    mesh_vertex_attribute_t attributes[MAX_ATTRIBUTE_COUNT];
} mesh_primitive_t;

#define MAX_PRIMITIVE_COUNT 8
typedef struct
{
    mesh_primitive_t primitives[MAX_PRIMITIVE_COUNT];
    u32 primitive_count;

    const char *name;
} mesh_t;


#define MAX_MATERIAL_COUNT 16
#define MAX_MESH_COUNT 8
#define MAX_ANIMATIONS_COUNT 8
typedef struct
{
    mesh_t meshes[MAX_MESH_COUNT];
    u32 mesh_count;

    mesh_material_t materials[MAX_MATERIAL_COUNT];
    u32 material_count;

    /* mesh_armature_t armature;

    mesh_animation_t animations[MAX_ANIMATIONS_COUNT];
    u32 animation_count;  */
} model_3D_t;

void model_3D_create_from_file(model_3D_t *model, const char *path);
void model_3D_delete(model_3D_t *model);
