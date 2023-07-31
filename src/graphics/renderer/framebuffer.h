#pragma once


typedef struct 
{
    uint32_t fbo;
    uint32_t rbo;
    vec2_t size;
    texture_t texture;
    bool filter;
}framebuffer_t;

void framebuffer_create(framebuffer_t* framebuffer, vec2_t size, bool filter);
void framebuffer_bind(framebuffer_t* framebuffer);
void framebuffer_unbind();
void framebuffer_delete(framebuffer_t* framebuffer);