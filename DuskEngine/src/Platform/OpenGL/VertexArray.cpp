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
	{
		LOG("Created VA");
		glGenVertexArrays(1, &ResourceID);
	}

	VertexArray::VertexArray(VertexArray& va) noexcept
		:m_VB(va.m_VB), m_IB(va.m_IB)
	{
		ResourceID = va.ResourceID;
	}

	VertexArray& VertexArray::operator=(VertexArray va) noexcept
	{
		ResourceID = va.ResourceID;

		m_VB = va.m_VB;
		m_IB = va.m_IB;

		return *this;
	}

	void VertexArray::Free()
	{
		glDeleteVertexArrays(1, &ResourceID);
		m_VB.Free();
		m_IB.Free();
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(ResourceID);
	}

	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void VertexArray::SetBuffer(VertexBuffer& vb)
	{
		m_VB = vb;
		m_VB.Bind();

		const auto& elements = m_VB.GetLayout().GetElements();
		unsigned int offset = 0;
		for (unsigned int i = 0; i < elements.size(); i++) {
			const auto& element = elements[i];
			glVertexAttribPointer(i, element.count, ShaderDataTypeToOpenGLBaseType(element.type), element.normalized,
				m_VB.GetLayout().GetStride(), (void*)(size_t)offset);
			offset += element.count * ShaderDataTypeSize(element.type);
			glEnableVertexAttribArray(i);
		}
	}

	void VertexArray::SetIndices(IndexBuffer& ib)
	{
		m_IB = ib;
		m_IB.Bind();
	}
}