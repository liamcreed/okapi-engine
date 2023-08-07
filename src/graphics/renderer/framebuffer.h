#pragma once


typedef struct 
{
    u32 fbo;
    u32 rbo;
    vec2_t resolution;
    texture_t texture;

    bool color, depth, stencil;
    i32 samples;
}framebuffer_t;

void framebuffer_create(framebuffer_t* framebuffer);
void framebuffer_bind(framebuffer_t* framebuffer);
void framebuffer_unbind();
void framebuffer_delete(framebuffer_t* framebuffer);