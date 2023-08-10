#pragma once

typedef struct mesh_joint_t mesh_joint_t;
struct mesh_joint_t
{
    u32 id;
    char name[32];
    mesh_joint_t* children;
    u32 child_count;
    mesh_joint_t* parent;
    mat4_t anim_transform;
    mat4_t model_transform;
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

#define MAX_KEY_FRAME_COUNT 32
typedef struct
{
    char name[64];
    key_frame_scale_t key_frame_scale[MAX_KEY_FRAME_COUNT];
    key_frame_trans_t key_frame_trans[MAX_KEY_FRAME_COUNT];
    key_frame_rot_t key_frame_rot[MAX_KEY_FRAME_COUNT];
} mesh_animation_t;

#define MAX_JOINT_COUNT 32
typedef struct
{
    char name[32];
    mesh_joint_t root_joint;
    u32 joint_count;
    mesh_joint_t* joints[MAX_JOINT_COUNT];
} mesh_armature_t;

void mesh_animation_play(mesh_armature_t* armature, mesh_animation_t* animation, f32 dt);

//-------------------------------------------------------------//

typedef struct
{
    texture_t diffuse_map;
    texture_t orm_map;
    texture_t normal_map;
    char name[32];
} mesh_material_t;

typedef struct
{
    char name[32];
    size_t size;
    size_t offset;
    size_t stride;
}mesh_vertex_attribute_t;

#define MAX_ATTRIBUTE_COUNT 7
typedef struct
{
    u32 material_index;
    vertex_array_t vertex_array;

    void* vertices;
    size_t vertices_size;

    void* indices;
    u32 index_count;
    u32 index_type;
    size_t indices_size;

    u32 attribute_count;
    mesh_vertex_attribute_t attributes[MAX_ATTRIBUTE_COUNT];
} mesh_primitive_t;

#define MAX_PRIMITIVE_COUNT 8
typedef struct
{
    mesh_primitive_t primitives[MAX_PRIMITIVE_COUNT];
    u32 primitive_count;

    char name[32];
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

    mesh_armature_t armature;

    mesh_animation_t animations[MAX_ANIMATIONS_COUNT];
    u32 animation_count;
} model_3D_t;

void model_3D_load_from_GLTF(model_3D_t* model, const char* path);
void model_3D_create(model_3D_t* model);
void model_3D_delete(model_3D_t* model);
