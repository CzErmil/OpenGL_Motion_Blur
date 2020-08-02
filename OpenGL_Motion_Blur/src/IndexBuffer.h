#pragma once

#include "GL/glew.h"

class IndexBuffer
{
private:
	GLuint m_ID;
	GLuint m_Count;
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	IndexBuffer(const unsigned int* data, unsigned int count, GLenum usage);
	~IndexBuffer();

	void Bind();
	void UnBind();

	void UpdateBuffer(const unsigned int* data, unsigned int count);

	inline GLuint GetCount() { return m_Count; };
};

