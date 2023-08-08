make a blender script that exports all neccerary data from a single mesh to fill these sttructs with data to
draw a 3d model on the screen with opengl.

typedef struct
{
    const char *name;
    mesh_material_t material;
    vertex_array_t vertex_array;

    void* vertices;
    size_t vertices_size;
    
    uin32_t* indices;
    size_t indices_size;
    uin32_t index_type;

    uin32_t attribute_count;
    mesh_vertex_attribute_t attributes[3]; //(position, normals and uv)

} mesh_t;

typedef struct
{
    uin32_t id;
    uin32_t width;
    uin32_t height;
    uin32_t channel_count;
    unsigned char* data;
    bool filter, sRGB;
} texture_t;

typedef struct
{
    texture_t diffuse_map;
    texture_t orm_map;
    texture_t normal_map;
    const char *name;
} mesh_material_t;

typedef struct 
{
    const char* name;
    size_t size;
    size_t offset;
    size_t stride;
}mesh_vertex_attribute_t;





