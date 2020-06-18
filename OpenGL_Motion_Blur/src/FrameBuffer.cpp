#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(Attachment attachment, int width, int height) :
    m_Width(width), m_Height(height)
{
    glGenFramebuffers(1, &m_ID);

    Bind();

    switch (attachment)
    {
    case Attachment::COLOR_RGBA8:
        m_Texture = std::make_unique<Texture>(m_Width, m_Height, GL_NEAREST, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture->GetID(), 0);
        break;
    case Attachment::COLOR_RGBA16F:
        m_Texture = std::make_unique<Texture>(m_Width, m_Height, GL_NEAREST, GL_RGBA16F, GL_RGBA, GL_UNSIGNED_BYTE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture->GetID(), 0);
        break;
    case Attachment::DEPTH:
        m_Texture = std::make_unique<Texture>(m_Width, m_Height, GL_NEAREST, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_Texture->GetID(), 0);

        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        break;
    default:
        break;
    }

    Unbind();
}

FrameBuffer::~FrameBuffer()
{

}

void FrameBuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
}

void FrameBuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::SetDepthBuffer()
{
    Bind();
    GLuint depthrenderbuffer;
    glGenRenderbuffers(1, &depthrenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Width, m_Height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

    GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, DrawBuffers);
    Unbind();
}