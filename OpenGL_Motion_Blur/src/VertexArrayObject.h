#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArrayObject
{
private:
	GLuint m_ID;
public:
	VertexArrayObject();
	~VertexArrayObject();

	void Bind() const;
	void Unbind() const;

	void AddVertexBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout);
};

