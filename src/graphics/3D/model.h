#pragma once

typedef struct mesh_joint_t mesh_joint_t;
struct mesh_joint_t
{
    u32 id;
    char name[32];

    i32 parent_id;

    vec4_t rotation;
    vec3_t location;

    mat4_t local_bind_matrix;
    mat4_t inverse_bind_matrix;
};

typedef struct
{
    vec4_t rotation;
    vec3_t location;
    f32 time_stamp;
} key_frame_t;


#define MAX_JOINT_COUNT 32
#define MAX_KEY_FRAME_COUNT 24 * 8

typedef struct
{
    char name[64];
    f32 duration;
    i32 frame_rate;

    key_frame_t (*key_frames)[MAX_KEY_FRAME_COUNT];
    u32 key_frame_count[MAX_JOINT_COUNT];
} mesh_animation_t;

typedef struct
{
    char name[32];
    u32 joint_count;
    mesh_joint_t joints[MAX_JOINT_COUNT];
    mat4_t joint_matrices[MAX_JOINT_COUNT];
} mesh_armature_t;

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
#define MAX_ANIMATIONS_COUNT 16
typedef struct
{
    u32 mesh_count;
    mesh_t meshes[MAX_MESH_COUNT];

    u32 material_count;
    mesh_material_t materials[MAX_MATERIAL_COUNT];

    mesh_armature_t armature;

    u32 animation_count;
    mesh_animation_t animations[MAX_ANIMATIONS_COUNT];
} model_3D_t;

void model_3D_load_from_GLTF(model_3D_t* model, const char* path);
void model_3D_create(model_3D_t* model);
void model_3D_delete(model_3D_t* model);
