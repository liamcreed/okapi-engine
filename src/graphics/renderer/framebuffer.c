#include "graphics/graphics.h"

void framebuffer_create(framebuffer_t* framebuffer)
{
    if (framebuffer->resolution.x == 0 || framebuffer->resolution.y == 0)
        printf(LOG_WARNING"[Framebuffer]: resolution is x: 0, y: 0 !!\n");

    printf("%i\n", framebuffer->color);
    if (framebuffer->color == false && framebuffer->depth == false && framebuffer->stencil == false)
        printf(LOG_WARNING"[Framebuffer]: no color, depth and stencil!!\n");

    glGenFramebuffers(1, &framebuffer->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->fbo);

    if (framebuffer->samples != 0)
    {
        glGenTextures(1, &framebuffer->texture.id);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, framebuffer->texture.id);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, framebuffer->resolution.x, framebuffer->resolution.y, GL_TRUE);

        i32 u = glGetError();
        if (u != 0)
        {
            printf(OPENGL_ERROR "%u\n", u);
            printf(OPENGL_ERROR "HFJSAHFLASKFL\n", u);
            // exit(-1);
        }

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, framebuffer->texture.id, 0);

        if (framebuffer->depth)
        {
            glGenRenderbuffers(1, &framebuffer->rbo);
            glBindRenderbuffer(GL_RENDERBUFFER, framebuffer->rbo);
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, framebuffer->samples, GL_DEPTH24_STENCIL8, framebuffer->resolution.x, framebuffer->resolution.y);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, framebuffer->rbo);
        }
    }
    else
    {
        glGenTextures(1, &framebuffer->texture.id);
        glBindTexture(GL_TEXTURE_2D, framebuffer->texture.id);

        if (framebuffer->color)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, framebuffer->resolution.x, framebuffer->resolution.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        else if (framebuffer->color)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, framebuffer->resolution.x, framebuffer->resolution.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebuffer->texture.id, 0);

        if (framebuffer->depth)
        {
            glGenRenderbuffers(1, &framebuffer->rbo);
            glBindRenderbuffer(GL_RENDERBUFFER, framebuffer->rbo);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, framebuffer->resolution.x, framebuffer->resolution.y);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, framebuffer->rbo);
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
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->fbo);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, framebuffer->resolution.x, framebuffer->resolution.y);
}
void framebuffer_unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void framebuffer_delete(framebuffer_t* framebuffer)
{
    glDeleteFramebuffers(1, &framebuffer->fbo);
    glDeleteRenderbuffers(1, &framebuffer->rbo);
    glDeleteTextures(1, &framebuffer->texture.id);
}