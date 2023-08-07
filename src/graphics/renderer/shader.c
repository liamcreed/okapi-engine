#include "graphics/graphics.h"

void shader_create(shader_t* shader)
{
    long length;
    FILE *v_file = fopen(shader->v_path, "rb");
    if (v_file == NULL)
        printf(LOG_ERROR "[Shader]: Failed to open: %s\n", shader->v_path);

    fseek(v_file, 0, SEEK_END);
    length = ftell(v_file);
    fseek(v_file, 0, SEEK_SET);
    char v_source[length+1];
    fread(v_source, 1, length, v_file);
    fclose(v_file);
    v_source[length] = '\0';

    length = 0;
    FILE *f_file = fopen(shader->f_path, "rb");
    if (f_file == NULL)
        printf(LOG_ERROR "[Shader]: Failed to open: %s\n", shader->f_path);

    fseek(f_file, 0, SEEK_END);
    length = ftell(f_file);
    fseek(f_file, 0, SEEK_SET);
    char f_source[length+1];
    fread(f_source, 1, length, f_file);
    fclose(f_file);
    f_source[length] = '\0';

    const char *v_src = v_source;
    const char *f_src = f_source;

    u32 v_shader;
    v_shader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(v_shader, 1, &v_src, NULL);
    glCompileShader(v_shader);

    i32 succ;
    char log[512];
    glGetShaderiv(v_shader, GL_COMPILE_STATUS, &succ);
    if (!succ)
    {
        glGetShaderInfoLog(v_shader, 512, NULL, log);
        printf(LOG_ERROR "[%s]: %s\n", shader->v_path, log);
        exit(-1);
    }

    u32 f_shader;
    f_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(f_shader, 1, &f_src, NULL);
    glCompileShader(f_shader);

    glGetShaderiv(f_shader, GL_COMPILE_STATUS, &succ);
    if (!succ)
    {
        glGetShaderInfoLog(f_shader, 512, NULL, log);
        printf(LOG_ERROR "[%s]: %s\n", shader->f_path, log);
        exit(-1);
    }

    shader->id = glCreateProgram();
    glAttachShader(shader->id, v_shader);
    glAttachShader(shader->id, f_shader);
    glLinkProgram(shader->id);
    glDeleteShader(v_shader);
    glDeleteShader(f_shader);

    glGetProgramiv(shader->id, GL_LINK_STATUS, &succ);
    if (!succ)
    {
        glGetProgramInfoLog(shader->id, 512, NULL, log);
        printf(LOG_ERROR "[shader]: %s\n", log);
        exit(-1);
    }
   
    glUseProgram(shader->id);
}
void shader_bind(shader_t *shader)
{
    glUseProgram(shader->id);
}
void shader_unbind()
{
    glUseProgram(0);
}
void shader_delete(shader_t *shader)
{
    glDeleteProgram(shader->id);
}

void shader_set_uniform_mat4(shader_t *shader, const char *name, mat4_t matrix)
{
    i32 loc = glGetUniformLocation(shader->id, name);
    if (loc == -1)
    {
        printf(LOG_ERROR "[shader]: no uniform with name %s found!\n", name);
        exit(-1);
    }
    
    glProgramUniformMatrix4fv(shader->id, loc, 1, GL_TRUE, &matrix.data[0][0]);
}
void shader_set_uniform_mat4_arr(shader_t *shader, const char *name, mat4_t* matrices, u32 count)
{
    i32 loc = glGetUniformLocation(shader->id, name);
    if (loc == -1)
    {
        printf(LOG_ERROR "[shader]: no uniform with name %s found!\n", name);
        exit(-1);
    }
    
    glProgramUniformMatrix4fv(shader->id, loc, count, GL_TRUE, &matrices->data[0][0]);
}

void shader_set_uniform_int(shader_t *shader, const char *name, i32 data)
{
    i32 loc = glGetUniformLocation(shader->id, name);
    if (loc == -1)
    {
        printf(LOG_ERROR "[shader]: no uniform with name %s found!\n", name);
        exit(-1);
    }
    glProgramUniform1i(shader->id, loc, data);
}

void shader_set_uniform_int_arr(shader_t *shader, const char *name, i32 *data, u32 count)
{
    i32 loc = glGetUniformLocation(shader->id, name);
    if (loc == -1)
    {
        printf(LOG_ERROR "[shader]: no uniform with name %s found!\n", name);
        exit(-1);
    }
    glProgramUniform1iv(shader->id, loc, count, data);
}
void shader_set_uniform_vec4(shader_t *shader, const char *name, vec4_t data)
{
    i32 loc = glGetUniformLocation(shader->id, name);
    if (loc == -1)
    {
        printf(LOG_ERROR "[shader]: no uniform with name %s found!\n", name);
        exit(-1);
    }
    glProgramUniform4f(shader->id, loc, data.x, data.y, data.z, data.w);
}

void shader_set_uniform_vec3(shader_t *shader, const char *name, vec3_t data)
{
    i32 loc = glGetUniformLocation(shader->id, name);
    if (loc == -1)
    {
        printf(LOG_ERROR "[shader]: no uniform with name %s found!\n", name);
        exit(-1);
    }
    glProgramUniform3f(shader->id, loc, data.x, data.y, data.z);
}
