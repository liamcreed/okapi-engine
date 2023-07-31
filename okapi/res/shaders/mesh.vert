#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_norm;
layout (location = 2) in vec2  a_uv;

out vec3 v_pos;
out vec3 v_norm;
out vec2  v_uv;


uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;



void main()
{
    gl_Position = u_proj * u_view * u_model * vec4(a_pos, 1.0);
    v_norm = mat3(transpose(inverse(u_model))) * a_norm; 
    v_uv = a_uv;
    v_pos = vec3(u_model * vec4(a_pos, 1.0));
}