#include "graphics/graphics.h"

void framebuffer_create(framebuffer_t *framebuffer, vec2_t size, bool filter)
{
    framebuffer->size.x = (int)size.x;
    framebuffer->size.y = (int)size.y;
    framebuffer->filter = filter;

    glGenFramebuffers(1, &framebuffer->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->fbo);

    texture_create_framebuffer_tex(&framebuffer->texture, framebuffer->size, framebuffer->filter);

    glGenRenderbuffers(1, &framebuffer->rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, framebuffer->rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, framebuffer->size.x, framebuffer->size.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, framebuffer->rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        printf(LOG_ERROR "Failed to create framebuffer!\n");
        exit(-1);
    }

    framebuffer_unbind();
}

void framebuffer_bind(framebuffer_t *framebuffer)
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->fbo);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, framebuffer->size.x, framebuffer->size.y);
}
void framebuffer_unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void framebuffer_delete(framebuffer_t *framebuffer)
{
    glDeleteFramebuffers(1, &framebuffer->fbo);
    glDeleteRenderbuffers(1, &framebuffer->rbo);
    texture_delete(&framebuffer->texture);
}