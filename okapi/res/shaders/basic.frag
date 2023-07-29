#version 330 core

in vec3 v_pos;
in vec3 v_norm;
in vec2  v_uv;


out vec4 f_color;

uniform sampler2D base_color;

void main()
{
    vec2 uv = vec2(v_uv.x, 1 - v_uv.y);

    vec3 light_color = vec3(1,1,1);
    vec3 light_pos = vec3(0,0,1);

    vec3 cam_pos = vec3(0,0,1);

    float ambient_strenght = 0.5;
    vec3 ambient = light_color * ambient_strenght;

    vec3 norm = normalize(v_norm);
    vec3 light_dir = normalize(light_pos - v_pos);  

    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diff * light_color;

    float specular_strenght = .2;

    vec3 view_dir = normalize(cam_pos - v_pos);    
    vec3 reflect_dir = reflect(-light_dir, norm);  

    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
    vec3 specular = specular_strenght * spec * light_color;   

    f_color = texture(base_color, uv) * vec4((ambient + diffuse/2 ), 1);
}