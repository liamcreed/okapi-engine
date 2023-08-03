#version 330 core
out vec4 f_color;
  
in vec2 v_uv;

uniform sampler2D u_texture;

float brightness = 1.2f;

void main()
{ 
    vec4 color = texture(u_texture, v_uv);
    f_color = color;
}
