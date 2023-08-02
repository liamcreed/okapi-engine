#version 330 core

in vec3 v_pos;
in vec3 v_norm;
in vec2 v_uv;

in mat3 v_tbn;
in vec3 v_tang_pos;
in vec3 v_tang_cam_pos;
in vec3 v_tang_light_pos;
in vec3 v_tang_dir_light_dir;

out vec4 f_color;

uniform sampler2D diffuse_map;
uniform sampler2D orm_map;
uniform sampler2D normal_map;

uniform vec3 cam_pos; 

void main()
{
    vec2 uv = vec2(v_uv.x, 1 - v_uv.y);

    if(texture(diffuse_map, uv).a == 0)
        discard;
    
    float roughness = texture(orm_map, uv).g;
    
    vec3 normal = texture(normal_map, uv).rgb;
    normal = normalize(normal * 2.0 - 1.0); 
   
    vec3 diffuse_color = texture(diffuse_map, uv).rgb;

    vec3 ambient = 0.1 * diffuse_color;

    // diffuse
    vec3 light_dir = normalize(-v_tang_dir_light_dir);
    float diff = max(dot(light_dir, normal), 0.0);
    vec3 diffuse = diff * diffuse_color;

    // specular
    vec3 cam_dir = normalize(v_tang_cam_pos - v_tang_pos);
    vec3 halfway_dir = normalize(light_dir + cam_dir);  
    float spec = pow(max(dot(normal, halfway_dir), 0.0), 32.0);

    vec3 specular = vec3(1 - roughness) * spec;
    vec3 result = ambient + diffuse + specular;

    float gamma = 2.2;
    f_color = vec4(pow(result, vec3(1.0/gamma)), 1);
}