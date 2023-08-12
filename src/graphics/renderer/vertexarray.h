#pragma once

typedef struct
{
    u32 vbo;
    u32 vao;
    u32 ibo;
} vertex_array_t;

void vertex_array_create(vertex_array_t *vertex_array);
void vertex_array_create_vbo(vertex_array_t *vertex_array, void *vertices, size_t vertices_size, bool dynamic);
void vertex_array_create_ibo(vertex_array_t *vertex_array, void *indices, size_t indices_size, bool dynamic);
void vertex_array_push_attribute_f(u32 index, size_t size, size_t stride, size_t offset);
void vertex_array_push_attribute_i(u32 index, size_t size, size_t stride, size_t offset);
void vertex_array_push_attribute_ub(u32 index, size_t size, size_t stride, size_t offset);
void vertex_array_bind(vertex_array_t *vertex_array);
void vertex_array_unbind();
void vertex_array_bind_buffers(vertex_array_t *vertex_array);
void vertex_array_unbind_buffers();


void vertex_array_delete(vertex_array_t *vertex_array);

void vertex_array_push_vertex_data(size_t size, void *data);
void vertex_array_push_index_data(size_t size, void *data);



