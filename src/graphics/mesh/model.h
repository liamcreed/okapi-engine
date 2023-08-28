#pragma once

typedef struct
{
    u32 id;
    char name[32];

    i32 parent_id;

    vec4 rotation;
    vec3 location;

    mat4 local_bind_matrix;
    mat4 inverse_bind_matrix;
}mesh_joint_t;

typedef struct
{
    vec4 rotation;
    vec3 location;
    f32 time_stamp;
} key_frame_t;



#define MAX_KEY_FRAME_COUNT 24 * 20

typedef struct
{
    char name[64];
    f32 duration;
    f32 frame_rate;

    key_frame_t(*key_frames)[MAX_KEY_FRAME_COUNT];
    u32* key_frame_count;
    u32 total_keyframe_count;
} mesh_animation_t;

typedef struct
{
    u32 joint_count;
    mesh_joint_t* joints;
    mat4* joint_matrices;

    f32 time;
    bool playing_anim;
    mesh_animation_t* animation;
} mesh_armature_t;

typedef struct
{
    texture_t diffuse_map;
    vec4 color;
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

#define MAX_VERTEX_ATTRIBUTE_COUNT 7
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
    mesh_vertex_attribute_t attributes[MAX_VERTEX_ATTRIBUTE_COUNT];
} mesh_primitive_t;
typedef struct
{
    char name[32];
    u32 primitive_count;
    mesh_primitive_t* primitives;
} mesh_t;

typedef struct
{
    u32 material_count;
    mesh_material_t* materials;
    
    u32 mesh_count;
    mesh_t* meshes;

    mesh_armature_t armature;

    u32 animation_count;
    mesh_animation_t* animations;
} model_3D_t;

void model_3D_load_from_GLTF(model_3D_t* model, const char* path);
void model_3D_export_to_bin(model_3D_t* model, const char* path);

void model_3D_load_from_bin(model_3D_t* model, const char* path);
void model_3D_create(model_3D_t* model);
void model_3D_delete(model_3D_t* model);
