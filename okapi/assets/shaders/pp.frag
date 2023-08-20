#version 330

out vec4 f_color;
in vec2 v_uv;

uniform sampler2D u_texture;

vec2 resolution = vec2(1280 * 2,720 * 2);

void main()
{
    vec4 color = texture(u_texture, v_uv);
    f_color = color;
}