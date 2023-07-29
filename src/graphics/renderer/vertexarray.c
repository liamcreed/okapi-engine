#include "graphics/graphics.h"

void vertex_array_create(vertex_array_t *vertex_array)
{
    glGenVertexArrays(1, &vertex_array->vao);
    glBindVertexArray(vertex_array->vao);

    if (vertex_array->vao == 0)
    {
        printf(LOG_ERROR "[OPENGL]: failed to create va!\n");
        exit(-1);
    }
}
void vertex_array_create_vbo(vertex_array_t *vertex_array, void *vertices, size_t vertices_size, bool dynamic)
{
    GLenum usage;
    if (dynamic)
        usage = GL_DYNAMIC_DRAW;
    else
        usage = GL_STATIC_DRAW;
    glGenBuffers(1, &vertex_array->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_array->vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, usage);

    if (vertex_array->vbo == 0)
    {
        printf(LOG_ERROR "[OPENGL]: failed to create vbo!\n");
        exit(-1);
    }
}
void vertex_array_create_ibo(vertex_array_t *vertex_array, void *indices, size_t indices_size, bool dynamic)
{
    GLenum usage;
    if (dynamic)
        usage = GL_DYNAMIC_DRAW;
    else
        usage = GL_STATIC_DRAW;
    glGenBuffers(1, &vertex_array->ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_array->ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, usage);

    if (vertex_array->ibo == 0)
    {
        printf(LOG_ERROR "[OPENGL]: failed to create ibo!\n");
        exit(-1);
    }
}
void vertex_array_bind(vertex_array_t *vertex_array)
{
    glBindVertexArray(vertex_array->vao);
}

void vertex_array_bind_buffers(vertex_array_t *vertex_array)
{
    glBindBuffer(GL_ARRAY_BUFFER, vertex_array->vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_array->ibo);
}

void vertex_array_unbind_buffers()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void vertex_array_unbind()
{
    glBindVertexArray(0);
}
void vertex_array_push_attribute(uint32_t index, uint32_t size, size_t stride, size_t offset)
{
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (void *)offset);
    glEnableVertexAttribArray(index);
}
void vertex_array_delete(vertex_array_t *vertex_array)
{
    glDeleteBuffers(1, &vertex_array->vbo);
    glDeleteBuffers(1, &vertex_array->ibo);
    glDeleteVertexArrays(1, &vertex_array->vao);
}

void vertex_array_push_vertex_data(size_t size, void *data)
{
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}
void vertex_array_push_index_data(size_t size, void *data)
{
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data);
}