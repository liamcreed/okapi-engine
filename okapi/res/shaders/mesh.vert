#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_norm;
layout (location = 2) in vec2 a_uv;
layout (location = 3) in vec4 a_tang;
layout (location = 4) in ivec4 a_joints;
layout (location = 5) in vec4 a_weights;

out vec3 v_pos;
out vec3 v_norm;
out vec2 v_uv;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

const int MAX_JOINTS = 32;
const int MAX_WEIGHTS = 4;

uniform mat4 joint_matrices[MAX_JOINTS];

void main()
{
    //v_pos = vec3(u_model * vec4(a_pos, 1.0));   
    v_pos = a_pos;

    vec4 total_local_pos = vec4(0);
    vec4 total_norm = vec4(0);

    
    for(int i = 0; i < MAX_WEIGHTS; i++)
    {
        mat4 joint_transform = joint_matrices[a_joints[i]];

        vec4 pose_pos = joint_transform * vec4(v_pos, 1.0);
        total_local_pos += pose_pos * a_weights[i];

        vec4 world_normal = joint_transform * vec4(a_norm, 1.0);
        total_norm += world_normal * a_weights[i];
    }

    v_norm = mat3(transpose(inverse(u_model))) * total_norm.xyz;
    
    
    
    v_uv = a_uv;

    gl_Position = u_proj * u_view * u_model  * total_local_pos;
}