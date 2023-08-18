#include "graphics/graphics.h"

void shader_compile(u32* id, const char* path, u32 type)
{
    u32 length = 0;
    FILE* file = fopen(path, "rb");
    if (file == NULL)
    {
        printf(LOG_ERROR "[Shader]: Failed to open: %s\n", path);
        exit(-1);
    }

    fseek(file, 0, SEEK_END);
    length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char source[length + 1];
    fread(source, 1, length, file);
    source[length] = '\0';
    fclose(file);
    const char* src = source;

    *id = glCreateShader(type);

    GL(glShaderSource(*id, 1, &src, NULL));
    GL(glCompileShader(*id));

    i32 succ;
    char log[512];
    GL(glGetShaderiv(*id, GL_COMPILE_STATUS, &succ));
    if (!succ)
    {
        GL(glGetShaderInfoLog(*id, 512, NULL, log));
        printf(LOG_ERROR "[%s]: %s\n", path, log);
        exit(-1);
    }
}
void shader_create(shader_t* shader)
{
    u32 v_shader = 0;
    u32 f_shader = 0;
    u32 g_shader = 0;
    if (shader->v_path)
        shader_compile(&v_shader, shader->v_path, GL_VERTEX_SHADER);
    if (shader->f_path)
        shader_compile(&f_shader, shader->f_path, GL_FRAGMENT_SHADER);
    if (shader->g_path != NULL)
        shader_compile(&g_shader, shader->g_path, GL_GEOMETRY_SHADER);

    shader->id = glCreateProgram();
    if (v_shader != 0)
    {
        GL(glAttachShader(shader->id, v_shader));
    }
    if (f_shader != 0)
    {
        GL(glAttachShader(shader->id, f_shader));
    }
    if (g_shader != 0)
    {
        GL(glAttachShader(shader->id, g_shader));
    }

    glLinkProgram(shader->id);
    GL(glDeleteShader(v_shader));
    GL(glDeleteShader(f_shader));
    GL(glDeleteShader(g_shader)); // IDK

    i32 succ;
    char log[512];
    GL(glGetProgramiv(shader->id, GL_LINK_STATUS, &succ));
    if (!succ)
    {
        GL(glGetProgramInfoLog(shader->id, 512, NULL, log));
        printf(LOG_ERROR "[shader]: %s\n", log);
        exit(-1);
    }

    GL(glUseProgram(shader->id));
}
void shader_bind(shader_t* shader)
{
    GL(glUseProgram(shader->id));
}
void shader_unbind()
{
    GL(glUseProgram(0));
}
void shader_delete(shader_t* shader)
{
    GL(glDeleteProgram(shader->id));
}

void shader_set_uniform_mat4(shader_t* shader, const char* name, mat4_t matrix)
{
    i32 loc = glGetUniformLocation(shader->id, name);
    if (loc == -1)
    {
        printf(LOG_ERROR "[shader]: no uniform with name %s found!\n", name);
        exit(-1);
    }

    GL(glProgramUniformMatrix4fv(shader->id, loc, 1, GL_TRUE, &matrix.data[0][0]));
}
void shader_set_uniform_mat4_arr(shader_t* shader, const char* name, mat4_t* matrices, u32 count)
{
    i32 loc = glGetUniformLocation(shader->id, name);
    if (loc == -1)
    {
        printf(LOG_ERROR "[shader]: no uniform with name %s found!\n", name);
        exit(-1);
    }

    GL(glProgramUniformMatrix4fv(shader->id, loc, count, GL_TRUE, &matrices->data[0][0]));
}

void shader_set_uniform_int(shader_t* shader, const char* name, i32 data)
{
    i32 loc = glGetUniformLocation(shader->id, name);
    if (loc == -1)
    {
        printf(LOG_ERROR "[shader]: no uniform with name %s found!\n", name);
        exit(-1);
    }
    GL(glProgramUniform1i(shader->id, loc, data));
}

void shader_set_uniform_int_arr(shader_t* shader, const char* name, i32* data, u32 count)
{
    i32 loc = glGetUniformLocation(shader->id, name);
    if (loc == -1)
    {
        printf(LOG_ERROR "[shader]: no uniform with name %s found!\n", name);
        exit(-1);
    }
    GL(glProgramUniform1iv(shader->id, loc, count, data));
}
void shader_set_uniform_vec4(shader_t* shader, const char* name, vec4_t data)
{
    i32 loc = glGetUniformLocation(shader->id, name);
    if (loc == -1)
    {
        printf(LOG_ERROR "[shader]: no uniform with name %s found!\n", name);
        exit(-1);
    }
    GL(glProgramUniform4f(shader->id, loc, data.x, data.y, data.z, data.w));
}

void shader_set_uniform_vec3(shader_t* shader, const char* name, vec3_t data)
{
    i32 loc = glGetUniformLocation(shader->id, name);
    if (loc == -1)
    {
        printf(LOG_ERROR "[shader]: no uniform with name %s found!\n", name);
        exit(-1);
    }
    GL(glProgramUniform3f(shader->id, loc, data.x, data.y, data.z));
   
}
