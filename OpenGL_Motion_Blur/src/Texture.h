#pragma once

#include "GL/glew.h"
#include <iostream>

class Texture {
private:
	GLuint m_ID;
	std::string m_FileName;
	GLuint m_Slot;
	int m_Width;
	int m_Height;
	int m_BytesPerPixel;
	unsigned char* m_LocalBuffer;
public:
	Texture(const std::string& fileName);
	Texture(const std::string& fileName, int parm);
	Texture(int width, int height, int param, int internalFormat, int format, int type);
	~Texture();

	void Bind(unsigned int slot = 0);
	void Unbind();

	inline unsigned int GetID() const { return m_ID; };
	inline int GetWidth() const { return m_Width; };
	inline int GetHeight() const { return m_Height; };
	inline int GetSlot() const { return m_Slot; };
};