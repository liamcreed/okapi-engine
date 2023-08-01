#version 330 core
out vec4 f_color;

in vec3 v_uv;

uniform samplerCube skybox;

void main()
{    
    f_color = texture(skybox, v_uv);
}