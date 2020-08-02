#pragma once

#include "GL/glew.h"

class VertexBuffer
{
private:
	GLuint m_ID;
	GLuint m_Size;
public:
	VertexBuffer(const void* data, unsigned int size);
	VertexBuffer(const void* data, unsigned int size, GLenum usage);
	~VertexBuffer();

	void Bind() const;
	void UnBind() const;

	void UpdateData(const void* data, unsigned int size);

	inline GLuint GetSize() const { return m_Size; }
};

