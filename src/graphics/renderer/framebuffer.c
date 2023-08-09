#include "graphics/graphics.h"

void framebuffer_create(framebuffer_t* framebuffer)
{
    if (framebuffer->texture.width == 0 || framebuffer->texture.height == 0)
        printf(LOG_WARNING"[Framebuffer]: resolution is x: 0, y: 0 !!\n");

    printf("%i\n", framebuffer->color);
    if (framebuffer->color == false && framebuffer->depth == false && framebuffer->stencil == false)
        printf(LOG_WARNING"[Framebuffer]: no color, depth and stencil!!\n");

    GL(glGenFramebuffers(1, &framebuffer->fbo));
    GL(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->fbo));

    if (framebuffer->samples != 0)
    {
        GL(glGenTextures(1, &framebuffer->texture.id));
        GL(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, framebuffer->texture.id));
        GL(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, framebuffer->texture.width, framebuffer->texture.height, GL_TRUE));

        GL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, framebuffer->texture.id, 0));

        if (framebuffer->depth)
        {
            GL(glGenRenderbuffers(1, &framebuffer->rbo));
            GL(glBindRenderbuffer(GL_RENDERBUFFER, framebuffer->rbo));
            GL(glRenderbufferStorageMultisample(GL_RENDERBUFFER, framebuffer->samples, GL_DEPTH24_STENCIL8, framebuffer->texture.width, framebuffer->texture.height));
            GL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, framebuffer->rbo));
        }
    }
    else
    {
        GL(glGenTextures(1, &framebuffer->texture.id));
        GL(glBindTexture(GL_TEXTURE_2D, framebuffer->texture.id));

        if (framebuffer->color)
        {
            GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, framebuffer->texture.width, framebuffer->texture.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
        }
        else if (framebuffer->color)
        {
            GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, framebuffer->texture.width, framebuffer->texture.height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
        }

        GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

        GL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebuffer->texture.id, 0));

        if (framebuffer->depth)
        {
            GL(glGenRenderbuffers(1, &framebuffer->rbo));
            GL(glBindRenderbuffer(GL_RENDERBUFFER, framebuffer->rbo));
            GL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, framebuffer->texture.width, framebuffer->texture.height));
            GL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, framebuffer->rbo));
        }
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        printf(LOG_ERROR "Failed to create framebuffer!\n");
        exit(-1);
    }

    framebuffer_unbind();
}

void framebuffer_bind(framebuffer_t* framebuffer)
{
    GL(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->fbo));
    GL(glEnable(GL_DEPTH_TEST));
    GL(glViewport(0, 0, framebuffer->texture.width, framebuffer->texture.height));
}
void framebuffer_unbind()
{
    GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}
void framebuffer_delete(framebuffer_t* framebuffer)
{
    GL(glDeleteFramebuffers(1, &framebuffer->fbo));
    GL(glDeleteRenderbuffers(1, &framebuffer->rbo));
    GL(glDeleteTextures(1, &framebuffer->texture.id));
}