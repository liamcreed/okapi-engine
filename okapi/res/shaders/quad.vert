#version 330 core
layout(location = 0) in vec3  a_pos;
layout(location = 1) in vec4  a_color;
layout(location = 2) in vec2  a_uv;
layout(location = 3) in float a_tex;

out vec3  v_pos;
out vec4  v_color;
out vec2  v_uv;
out float v_tex;

uniform mat4 u_proj_mat;
uniform mat4 u_view_mat;

void main()
{
    gl_Position = u_proj_mat * u_view_mat  * vec4(a_pos, 1.0);

    v_pos    = a_pos;
    v_color  = a_color;
    v_uv     = a_uv;
    v_tex    = a_tex;
}