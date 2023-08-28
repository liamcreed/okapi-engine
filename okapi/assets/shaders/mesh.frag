#version 330 core

in vec3 v_pos;
in vec3 v_norm;
in vec2 v_uv;

out vec4 f_color;

uniform sampler2D u_diffuse_map;
uniform vec4 u_color;
uniform sampler2D u_orm_map;

uniform vec3 u_light_dir;

void main()
{
    if(texture(u_diffuse_map, v_uv).a < 0.1)
        discard;

    float roughness = texture(u_orm_map, v_uv).g;

    vec3 color = u_color.xyz;
            
    vec3 diffuse_color = texture(u_diffuse_map, v_uv).rgb * color;

    float ambient_factor = 0.7;
    vec3 ambient = diffuse_color * ambient_factor;

    vec3 light_dir = normalize(-u_light_dir.xyz);
    float diff =  max(dot(v_norm, light_dir), 0.0);

    vec3 diffuse =  diff * diffuse_color;

      
    vec3 result = ambient + diffuse;
    
    float gamma = 2.2;
    f_color.rgb = pow(result, vec3(1.0/gamma)); 
    f_color.a = texture(u_diffuse_map, v_uv).a;
}