#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(Attachment attachment, int width, int height) :
	FrameBuffer(attachment, 1, width, height) {}

FrameBuffer::FrameBuffer(Attachment attachment, unsigned int textureCount, int width, int height) :
	m_TextureCount(textureCount), m_Width(width), m_Height(height)
{
	glGenFramebuffers(1, &m_ID);

	Bind();

	m_Target = m_TextureCount > 1 ? GL_TEXTURE_2D_ARRAY : GL_TEXTURE_2D;

	switch (attachment)
	{
	case Attachment::COLOR_RGBA8:
		m_ColorTexture = std::make_unique<Texture>(m_Target, m_TextureCount, m_Width, m_Height, GL_NEAREST, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTexture->GetID(), 0);
		break;
	case Attachment::COLOR_RGBA16F:
		m_ColorTexture = std::make_unique<Texture>(m_Target, m_TextureCount, m_Width, m_Height, GL_NEAREST, GL_RGBA16F, GL_RGBA, GL_UNSIGNED_BYTE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTexture->GetID(), 0);
		break;
	case Attachment::DEPTH:
		m_ColorTexture = std::make_unique<Texture>(m_Target, m_TextureCount, m_Width, m_Height, GL_NEAREST, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_ColorTexture->GetID(), 0);

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
	m_DepthTexture = std::make_unique<Texture>(m_Target, m_TextureCount, m_Width, m_Height, GL_NEAREST, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture->GetID(), 0);
	Unbind();
}