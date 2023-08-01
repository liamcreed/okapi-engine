#version 330 core

in vec3 v_pos;
in vec3 v_norm;
in vec2 v_uv;

in mat3 v_tbn;
in vec3 v_tang_pos;
in vec3 v_tang_cam_pos;
in vec3 v_tang_light_pos;

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
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
   
    // get diffuse color
    vec3 color = texture(diffuse_map, uv).rgb;
    // ambient
    vec3 ambient = 0.1 * color;
    // diffuse
    vec3 lightDir = normalize(v_tang_light_pos - v_tang_pos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular
    vec3 viewDir = normalize(v_tang_cam_pos - v_tang_pos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(1 - roughness) * spec;

    vec3 result = ambient + diffuse + specular;

    float gamma = 2.2;
    f_color = vec4(pow(result, vec3(1.0/gamma)), 1);
}