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
    
    if(texture(u_diffuse_map, v_uv).a < 1)
        discard;
    
    float roughness = texture(u_orm_map, v_uv).g;
    
    vec3 normal = texture(u_normal_map, v_uv).rgb;
    normal = normalize(normal * 2.0 - 1.0); 
   
    //vec3 diffuse_color = texture(u_diffuse_map, v_uv).rgb;
    vec3 diffuse_color = vec3(.4,.3,.4);

    vec3 ambient = 0. * diffuse_color;

    vec3 light_dir = normalize(-vec3(-1,0, -1));
    float diff = max(dot(v_norm, light_dir), 0.0);

    if(diff > 0.3)
        diff = 0.5;
    else if( diff > 0.1)
        diff = 0.1;
    else if(diff < 0.1)
        diff = 0;
    

        
    
    vec3 diffuse = diff * diffuse_color;

    vec3 result = ambient + diffuse;

    float gamma = 2.2;
    f_color.rgb = pow(result, vec3(1.0/gamma));
    f_color.a = texture(u_diffuse_map, v_uv).a;

    f_color.a = 1;

}