#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) :m_Count(count)
{
    glGenBuffers(1, &m_ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLint), data, GL_STATIC_DRAW);
}

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count, GLenum usage) :m_Count(count)
{
    glGenBuffers(1, &m_ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLint), data, usage);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &m_ID);
}

void IndexBuffer::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}

void IndexBuffer::UnBind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void IndexBuffer::UpdateBuffer(const unsigned int* data, unsigned int count)
{
    m_Count = count;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, count * sizeof(GLint), data);
}