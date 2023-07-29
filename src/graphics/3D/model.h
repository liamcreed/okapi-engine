#pragma once

typedef struct
{
    vec3_t pos;
    vec3_t normal;
    vec2_t uv;
} mesh_vertex_t;

typedef struct
{
    texture_t base_color;
    float roughness;
    const char* name;
} material_t;

typedef struct
{
    material_t* material;
    vertex_array_t vertex_array;
    uint32_t index_offset;
    uint32_t index_count;
} primitive_t;

typedef struct
{
    mat4_t transform;
    primitive_t* primitives;
    uint32_t primitive_count;   

    const char* name;
} mesh_t;

typedef struct
{
    mesh_t* meshes;
    uint32_t mesh_count;

    material_t* materials;
    mat4_t transform;
} model_3D_t;

void model_3D_create_from_file(model_3D_t *model, const char *file);
void model_3D_delete(model_3D_t *model);

