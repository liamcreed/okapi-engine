#version 330 core

in vec3  v_pos;
in vec2  v_uv;

out vec4 f_color;

uniform sampler2D u_texture;
uniform vec4 u_color;

void main()
{
    vec4 tex_color = texture(u_texture, v_uv);
    f_color = tex_color;
    float gamma = 2.2;
    f_color.rgb = pow(f_color.rgb, vec3(1.0/gamma));
    f_color *= u_color;
}