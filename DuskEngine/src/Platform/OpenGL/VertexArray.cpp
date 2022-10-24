#include "pch.h"
#include "VertexArray.h"

#include "GLCommon.h"
#include "Buffer.h"

namespace DuskEngine
{
	GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return GL_FLOAT;
		case ShaderDataType::Float2:   return GL_FLOAT;
		case ShaderDataType::Float3:   return GL_FLOAT;
		case ShaderDataType::Float4:   return GL_FLOAT;
		case ShaderDataType::Mat3:     return GL_FLOAT;
		case ShaderDataType::Mat4:     return GL_FLOAT;
		case ShaderDataType::Int:      return GL_INT;
		case ShaderDataType::Int2:     return GL_INT;
		case ShaderDataType::Int3:     return GL_INT;
		case ShaderDataType::Int4:     return GL_INT;
		case ShaderDataType::Bool:     return GL_BOOL;
		}
		return 0;
	}

	VertexArray::VertexArray()
		:m_ID(-1)
	{
		glGenVertexArrays(1, &m_ID);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_ID);
	}

	VertexArray::VertexArray(VertexArray&& va)
	{
		m_ID = va.m_ID;

		m_VertexBuffer = std::move(va.m_VertexBuffer);
		m_IndexBuffer = std::move(va.m_IndexBuffer);
	}

	VertexArray& VertexArray::operator=(VertexArray&& va)
	{
		m_ID = va.m_ID;

		m_VertexBuffer = std::move(va.m_VertexBuffer);
		m_IndexBuffer = std::move(va.m_IndexBuffer);
		return *this;
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(m_ID);
	}

	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void VertexArray::AddBuffer(UniqueRef<VertexBuffer>& vb)
	{
		vb->Bind();
		m_VertexBuffer = std::move(vb);

		const auto& elements = m_VertexBuffer->GetLayout()->GetElements();
		unsigned int offset = 0;
		for (unsigned int i = 0; i < elements.size(); i++) {
			const auto& element = elements[i];
			glVertexAttribPointer(i, element.count, ShaderDataTypeToOpenGLBaseType(element.type), element.normalized,
				m_VertexBuffer->GetLayout()->GetStride(), (void*)(size_t)offset);
			offset += element.count * ShaderDataTypeSize(element.type);
			glEnableVertexAttribArray(i);
		}
	}

	void VertexArray::AddIndices(UniqueRef<IndexBuffer>& ib)
	{
		ib->Bind();
		m_IndexBuffer = std::move(ib);
	}
}