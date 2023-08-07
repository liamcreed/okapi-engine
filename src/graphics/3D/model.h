#pragma once

typedef struct mesh_joint_t mesh_joint_t;
struct mesh_joint_t
{
    uint32_t id;
    char *name;
    mesh_joint_t *children;
    uint32_t child_count;
    mesh_joint_t *parent;
    mat4_t inverse_bind_transform;
};

typedef struct
{
    vec3_t scale;
    float time;
} key_frame_scale_t;

typedef struct
{
    quat_t rotation;
    float time;
} key_frame_rot_t;

typedef struct
{
    vec3_t position;
    float time;
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
    uint32_t joint_count;
} mesh_armature_t;

void mesh_animation_play(mesh_armature_t *armature, mesh_animation_t *animation, float dt);


typedef struct
{
    texture_t diffuse_map;
    texture_t orm_map;
    texture_t normal_map;
    const char *name;
} mesh_material_t;

typedef struct
{
    vec3_t position;
    vec3_t normal;
    vec2_t uv;
    //vec4_t tangent;
    //vec4_t joint_id;
    //vec4_t weights;
} mesh_vertex_t;


typedef struct
{
    mesh_material_t *material;
    vertex_array_t vertex_array;

    mesh_vertex_t* vertices;
    uint32_t vertex_count;
    
    uint32_t* indices;
    uint32_t index_count;
    uint32_t index_type;

    uint32_t attribute_count;
} mesh_primitive_t;

#define MAX_PRIMITIVE_COUNT 8
typedef struct
{
    mesh_primitive_t primitives[MAX_PRIMITIVE_COUNT];
    uint32_t primitive_count;

    const char *name;
} mesh_t;

#define MAX_MATERIAL_COUNT 16
#define MAX_MESH_COUNT 8
#define MAX_ANIMATIONS_COUNT 8
typedef struct
{
    mesh_t meshes[MAX_MESH_COUNT];
    uint32_t mesh_count;

    mesh_material_t materials[MAX_MATERIAL_COUNT];
    uint32_t material_count;
    mat4_t transform;

    mesh_armature_t armature;

    mesh_animation_t animations[MAX_ANIMATIONS_COUNT];
    uint32_t animation_count;
} model_3D_t;

void model_3D_create_from_file(model_3D_t *model, const char *path);
void model_3D_delete(model_3D_t *model);
