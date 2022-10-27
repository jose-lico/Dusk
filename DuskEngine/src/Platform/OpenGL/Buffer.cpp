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

	VertexBuffer::VertexBuffer()
	{
	}

	void VertexBuffer::Free()
	{
		glDeleteBuffers(1, &ResourceID);
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, ResourceID);
	}

	void VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::SetData(const void* data, unsigned int size)
	{
		LOG("Created VB");
		LOG("Set Data VB");
		glGenBuffers(1, &ResourceID);
		glBindBuffer(GL_ARRAY_BUFFER, ResourceID);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	// #############################################################
	// INDEX
	// #############################################################

	IndexBuffer::IndexBuffer()
		:m_Count(0)
	{

	}

	void IndexBuffer::Free()
	{
		glDeleteBuffers(1, &ResourceID);
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ResourceID);
	}

	void IndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void IndexBuffer::SetData(const void* data, unsigned int count)
	{
		LOG("Created IB");
		LOG("Set Data IB");
		m_Count = count;
		glGenBuffers(1, &ResourceID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ResourceID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
	}
}