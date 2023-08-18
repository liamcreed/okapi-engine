#version 330 core
layout(location = 0) in vec3  a_pos;
layout(location = 2) in vec2  a_uv;

out vec3  v_pos;
out vec4  v_color;
out vec2  v_uv;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;



void main()
{
    gl_Position = u_proj * u_view * u_model * vec4(a_pos, 1.0);

    v_pos = a_pos;
    v_uv = a_uv;
}