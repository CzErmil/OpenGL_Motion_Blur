#include "VertexArrayObject.h"

VertexArrayObject::VertexArrayObject()
{
	glGenVertexArrays(1, &m_ID);
}

VertexArrayObject::~VertexArrayObject()
{
	glDeleteVertexArrays(1, &m_ID);
}

void VertexArrayObject::Bind() const
{
	glBindVertexArray(m_ID);
}

void VertexArrayObject::Unbind() const
{
	glBindVertexArray(0);
}

void VertexArrayObject::AddVertexBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout)
{
	vertexBuffer.Bind();
	const auto& elements = layout.GetElement();
	GLuint index = 0,offset = 0;
	for(auto &element : elements)
	{
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, element.count, element.type, element.normalized, layout.GetStride() , (const void*)offset);
		offset += VertexBufferElement::GetSizeOfType(element.type)*element.count;
		index++;
	}
}