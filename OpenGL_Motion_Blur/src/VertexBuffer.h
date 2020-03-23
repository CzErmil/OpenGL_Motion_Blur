#include "GL/glew.h"

class VertexBuffer
{
private:
	GLuint m_ID;
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void UnBind() const;
};

