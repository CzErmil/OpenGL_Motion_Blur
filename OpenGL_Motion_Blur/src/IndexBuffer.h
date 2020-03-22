#include "GL/glew.h"

class IndexBuffer
{
private:
	GLuint m_ID;
	GLuint m_Count;
public:
	IndexBuffer(const GLuint* data, unsigned int count);
	~IndexBuffer();

	void Bind();
	void UnBind();

	inline GLuint GetCount() { return m_Count; };
};

