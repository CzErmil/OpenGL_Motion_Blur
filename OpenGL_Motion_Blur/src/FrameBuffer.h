#pragma once

#include "GL/glew.h"
#include <memory>
#include "Texture.h"

enum class Attachment {
	COLOR_RGBA8,
	COLOR_RGBA16F,
	DEPTH
};

class FrameBuffer
{
private:
	unsigned int m_ID;
	unsigned int m_TextureCount;
	int m_Width, m_Height;
	int m_Target;
	std::unique_ptr<Texture> m_ColorTexture;
	std::unique_ptr<Texture> m_DepthTexture;
public:
	FrameBuffer(Attachment attachment, int width, int height);
	FrameBuffer(Attachment attachment, unsigned int texturCount, int width, int height);
	~FrameBuffer();

	void Bind();
	void Unbind();

	inline void BindTexture(unsigned int slot = 0) { m_ColorTexture->Bind(slot); };
	inline void UnbindTexture() { m_ColorTexture->Unbind(); };

	void SetDepthBuffer();
};

