#include "IndexBuffer.h"
#include "GLCommon.h"

DuskEngine::IndexBuffer::IndexBuffer(unsigned int count, const void* data)
	:m_Count(count)
{
	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

DuskEngine::IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_ID);
}

void DuskEngine::IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}

void DuskEngine::IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}