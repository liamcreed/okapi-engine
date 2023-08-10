#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_norm;
layout (location = 2) in vec2 a_uv;
layout (location = 3) in vec4 a_tang;
layout (location = 4) in ivec4 a_joints;
layout (location = 4) in vec4 a_weights;

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
    vec4 total_local_pos = vec4(0);
    for(int i = 0; i < MAX_WEIGHTS; i++)
    {
        mat4 joint_transform = joint_matrices[a_joints[i]];
        vec4 pose_pos = joint_transform * vec4(a_pos, 1.0);
        total_local_pos += pose_pos * 0.25;
    }
    
    v_pos = vec3(u_model * vec4(a_pos, 1.0));   
    v_uv = a_uv;
    v_norm = mat3(transpose(inverse(u_model))) * a_norm;

    gl_Position = u_proj * u_view * u_model  * total_local_pos;
}