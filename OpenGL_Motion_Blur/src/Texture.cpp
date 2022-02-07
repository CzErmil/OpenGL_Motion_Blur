#include "Texture.h"

#include "Vendor/stb_image/stb_image.h"

Texture::Texture(const std::string& fileName)
	: Texture(fileName, GL_NEAREST) {}

Texture::Texture(const std::string& fileName, int parm)
	: m_ID(0), m_FileName(fileName), m_Slot(0), m_Width(0), m_Height(0), m_BytesPerPixel(0), m_LocalBuffer(nullptr)
{
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(m_FileName.c_str(), &m_Width, &m_Height, &m_BytesPerPixel, 4);

	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_2D, m_ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, parm);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, parm);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	if (m_LocalBuffer)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
}

Texture::Texture(int width, int height, int param, int internalFormat, int format, int type)
	: Texture(GL_TEXTURE_2D, 1, m_Width, m_Width, param, internalFormat, format, type) {}

Texture::Texture(int target, unsigned int textureCount, int width, int height, int param, int internalFormat, int format, int type)
	: m_ID(0), m_Count(textureCount), m_FileName(""), m_Slot(0), m_Width(width), m_Height(height), m_BytesPerPixel(0), m_LocalBuffer(nullptr)
{
	glGenTextures(1, &m_ID);
	glBindTexture(target, m_ID);

	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, param);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, param);
	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	if (m_Count > 1)
	{
		glTexImage3D(target, 0, internalFormat, m_Width, m_Height, m_Count, 0, format, type, m_LocalBuffer);
	}
	else
	{
		glTexImage2D(target, 0, internalFormat, m_Width, m_Height, 0, format, type, m_LocalBuffer);
	}
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_ID);
}

void Texture::Bind(unsigned int slot)
{
	_ASSERT(0 <= slot && slot <= 31);

	m_Slot = slot;
	glActiveTexture(GL_TEXTURE0 + slot);
	if (m_Count > 1)
	{
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_ID);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}
}

void Texture::Unbind()
{
	if (m_Count > 1)
	{
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}