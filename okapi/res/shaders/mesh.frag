#version 330 core

in vec3 v_pos;
in vec3 v_norm;
in vec2  v_uv;

out vec4 f_color;

uniform sampler2D diffuse_map;
uniform sampler2D specular_map;
uniform sampler2D normal_map;

void main()
{
    vec2 uv = vec2(v_uv.x, 1 - v_uv.y);

    if(texture(diffuse_map, uv).a == 0)
        discard;

    float ambient_strenght = 0.1;
    vec3 ambient = ambient_strenght * texture(diffuse_map, uv).rgb;
  	
    vec3 light_direction = vec3(0,-10,-10);
    
    light_direction = normalize(-light_direction); 
    float diff = 0.5 * max(dot(v_norm, light_direction), 0.0);

    vec3 diffuse = diff * texture(diffuse_map, uv).rgb;  
    
    vec3 result = ambient + diffuse;
    f_color = vec4(result, 1);

    float gamma = 2.2;
    f_color.rgb = pow(f_color.rgb, vec3(1.0/gamma));
}