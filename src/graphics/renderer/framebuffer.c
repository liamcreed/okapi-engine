#include "graphics/graphics.h"

void framebuffer_create(framebuffer_t *framebuffer, vec2_t size, bool color, bool depth, bool stencil, int samples)
{
    framebuffer->size.x = (int)size.x;
    framebuffer->size.y = (int)size.y;

    glGenFramebuffers(1, &framebuffer->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->fbo);

    if (samples != 0)
    {
        glGenTextures(1, &framebuffer->texture.id);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, framebuffer->texture.id);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, framebuffer->size.x, framebuffer->size.y, GL_TRUE);

        int u = glGetError();
        if (u != 0)
        {
            printf(OPENGL_ERROR "%u\n", u);
            printf(OPENGL_ERROR "HFJSAHFLASKFL\n", u);
            // exit(-1);
        }

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, framebuffer->texture.id, 0);

        if (depth)
        {
            glGenRenderbuffers(1, &framebuffer->rbo);
            glBindRenderbuffer(GL_RENDERBUFFER, framebuffer->rbo);
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, framebuffer->size.x, framebuffer->size.y);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, framebuffer->rbo);
        }
    }
    else
    {
        glGenTextures(1, &framebuffer->texture.id);
        glBindTexture(GL_TEXTURE_2D, framebuffer->texture.id);
        
        if (color)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, framebuffer->size.x, framebuffer->size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        else if (depth)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, framebuffer->size.x, framebuffer->size.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebuffer->texture.id, 0);

        if (depth)
        {
            glGenRenderbuffers(1, &framebuffer->rbo);
            glBindRenderbuffer(GL_RENDERBUFFER, framebuffer->rbo);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, framebuffer->size.x, framebuffer->size.y);
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
    glDeleteTextures(1, &framebuffer->texture.id);
}