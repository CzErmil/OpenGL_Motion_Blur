#pragma once

#include "GL/glew.h"

class VertexBuffer
{
private:
	GLuint m_ID;
	GLuint m_Size;
public:
	VertexBuffer();
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void UnBind() const;

	inline GLuint GetSize() const { return m_Size; }
};

