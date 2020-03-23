#pragma once

#include "GL/glew.h"
#include "vector"

struct VertexBufferElement
{
	GLenum type;
	GLuint count;
	GLboolean normalized;

	static unsigned int GetSizeOfType(GLenum type)
	{
		switch (type)
		{
		case GL_FLOAT:
			return sizeof(GLfloat);
		}
		_ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	GLsizei m_Stride;
public:
	VertexBufferLayout()
		:m_Stride(0) {}

	template<typename T>
	void Push(const GLuint count)
	{
		static_assert(false, "This type is not supported");
	}

	template<>
	void Push<GLfloat>(const GLuint count)
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * sizeof(GLfloat);
	}

	inline std::vector<VertexBufferElement> GetElement() const { return m_Elements; }
	inline GLsizei GetStride() const { return m_Stride; }
};

