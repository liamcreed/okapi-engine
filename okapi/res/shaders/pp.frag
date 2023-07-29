#version 330 core
out vec4 color;

in vec2 v_uv;

uniform sampler2D buffer_texture;

void main()
{
    vec4 tex_color = texture(buffer_texture, v_uv);
    if(tex_color.a < 0.1)
        discard;
    color = tex_color;
} 