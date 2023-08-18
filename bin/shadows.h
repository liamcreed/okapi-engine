
u32 shadow_fbo;
GL(glGenFramebuffers(1, &shadow_fbo));

#define SHADOW_WIDTH 2048 * 2
#define SHADOW_HEIGHT 2048 * 2

unsigned int shadow_map;
GL(glGenTextures(1, &shadow_map));
GL(glBindTexture(GL_TEXTURE_2D, shadow_map));
GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
    SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
float border_color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GL(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color));

GL(glBindFramebuffer(GL_FRAMEBUFFER, shadow_fbo));
GL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadow_map, 0));
GL(glDrawBuffer(GL_NONE));
GL(glReadBuffer(GL_NONE));
if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
{
    printf(LOG_ERROR "Failed to create framebuffer!\n");
    exit(-1);
}
GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));





update()
{
    //rendering depth
    GL(glEnable(GL_DEPTH_TEST));

    GL(glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT));
    GL(glBindFramebuffer(GL_FRAMEBUFFER, shadow_fbo));
    GL(glClear(GL_DEPTH_BUFFER_BIT));
    GL(glCullFace(GL_FRONT));

    mat4_t shadow_proj_mat = mat4_ortho_aspect(1, 10, -100, 100);
    mat4_t shadow_view_mat = mat4_look_at(vec3_add(player.position, vec3_scale(sun.direction, -5)), vec3_add(player.position, (vec3_t) { 0, 0, 0 }), (vec3_t) { 0, 1, 0 });

    shader_set_uniform_mat4(&renderer.mesh_shader, "u_view", shadow_view_mat);
    shader_set_uniform_mat4(&renderer.mesh_shader, "u_proj", shadow_proj_mat);
    shader_set_uniform_int(&renderer.mesh_shader, "u_shadow_pass", true);

    renderer_draw_model_3D(&renderer, &camera, &player_model, player.position, player.scale, player.rotation);
    renderer_draw_model_3D(&renderer, &camera, &landscape, (vec3_t) { 0, 0, 1 }, 1, (vec4_t) { 0, 0, 0, 1 });













    shader_set_uniform_mat4(&renderer.mesh_shader, "u_shadow_view", shadow_view_mat);
    shader_set_uniform_mat4(&renderer.mesh_shader, "u_shadow_proj", shadow_proj_mat);
    shader_set_uniform_int(&renderer.mesh_shader, "u_shadow_pass", false);
    shader_set_uniform_int(&renderer.mesh_shader, "u_shadow_map", 15);


    
        GL(glActiveTexture(GL_TEXTURE15));
        GL(glBindTexture(GL_TEXTURE_2D, shadow_map));
}



