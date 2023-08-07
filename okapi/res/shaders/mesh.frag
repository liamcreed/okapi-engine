#version 330 core

in vec3 v_pos;
in vec3 v_norm;
in vec2 v_uv;

out vec4 f_color;

uniform sampler2D u_diffuse_map;
uniform sampler2D u_orm_map;
uniform sampler2D u_normal_map;

void main()
{
    vec2 uv = vec2(v_uv.x, 1 - v_uv.y);
    if(texture(u_diffuse_map, uv).a < 1)
        discard;
    
    float roughness = texture(u_orm_map, uv).g;
    
    vec3 normal = texture(u_normal_map, uv).rgb;
    normal = normalize(normal * 2.0 - 1.0); 
   
    vec3 diffuse_color = texture(u_diffuse_map, uv).rgb;

    vec3 ambient = 0.7 * diffuse_color;

    vec3 light_dir = normalize(-vec3(-1,0, -1));
    float diff = max(dot(v_norm, light_dir), 0.0);

    float max = 0.2;
    if(diff > max)
        diff = 0.3;
    else if(diff < max)
        diff = 0;
    
    vec3 diffuse = diff * diffuse_color;

    vec3 result = ambient + diffuse;

    float gamma = 2.2;
    f_color = vec4(pow(result, vec3(1.0/gamma)), texture(u_diffuse_map, uv).a);
    
}