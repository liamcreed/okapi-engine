#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_norm;
layout (location = 2) in vec2 a_uv;
layout (location = 3) in vec4 a_tang;

out vec3 v_pos;
out vec3 v_norm;
out vec2 v_uv;

out mat3 v_tbn;
out vec3 v_tang_pos;
out vec3 v_tang_cam_pos;
out vec3 v_tang_light_pos;
out vec3 v_tang_dir_light_dir;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

uniform vec3 cam_pos; 

void main()
{
    
    v_pos = vec3(u_model * vec4(a_pos, 1.0));   
    v_uv = a_uv;
    
    mat3 norm_mat = transpose(inverse(mat3(u_model)));
    vec3 T = normalize(norm_mat * a_tang.xyz);
    vec3 N = normalize(norm_mat * a_norm);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    
    mat3 TBN = transpose(mat3(T, B, N));    
    v_tang_cam_pos  = TBN * cam_pos;
    v_tang_light_pos = TBN * vec3(0,10,10);
    v_tang_dir_light_dir = TBN * vec3(0,-1,1);
    v_tang_pos  = TBN * v_pos;

    gl_Position = u_proj * u_view * u_model * vec4(a_pos, 1.0);
}