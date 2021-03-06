#include "pch.h"
#include "Buffer.h"

#include "GLCommon.h"

namespace DuskEngine
{
	// #############################################################
	// LAYOUT
	// #############################################################

	void VertexBufferLayout::Push(ShaderDataType type, unsigned int count, bool normalized)
	{
		m_Elements.push_back({ type, count, normalized });
		m_Stride += ShaderDataTypeSize(type) * count;
	}

	// #############################################################
	// VERTEX
	// #############################################################

	VertexBuffer::VertexBuffer(const void* data, unsigned int size)
		:m_ID(0)
	{
		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_ID);
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	}

	void VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// #############################################################
	// INDEX
	// #############################################################

	IndexBuffer::IndexBuffer(const void* data, unsigned int count)
		:m_ID(0), m_Count(count)
	{
		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_ID);
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	}

	void IndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}