#version 330 core

in vec3  v_pos;
in vec4  v_color;
in vec2  v_uv;
in float v_tex;

out vec4 color;

uniform sampler2D u_textures[16];

void main()
{
    vec4 tex_color = texture(u_textures[int(v_tex)], v_uv) * v_color;
    if(tex_color.a < 0.1)
        discard;

    color = tex_color;
}