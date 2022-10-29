#include "pch.h"
#include "VertexArray.h"

#include "GLCommon.h"
#include "OpenGLAPI.h"
#include "Buffer.h"

namespace DuskEngine
{
	GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::FLOAT:    return GL_FLOAT;
		case ShaderDataType::FLOAT2:   return GL_FLOAT;
		case ShaderDataType::FLOAT3:   return GL_FLOAT;
		case ShaderDataType::FLOAT4:   return GL_FLOAT;
		case ShaderDataType::MAT3:     return GL_FLOAT;
		case ShaderDataType::MAT4:     return GL_FLOAT;
		case ShaderDataType::INT:      return GL_INT;
		case ShaderDataType::INT2:     return GL_INT;
		case ShaderDataType::INT3:     return GL_INT;
		case ShaderDataType::INT4:     return GL_INT;
		case ShaderDataType::BOOL:     return GL_BOOL;
		}
		return 0;
	}

	VertexArray::VertexArray()
	{
		LOG("Created VA");
		glGenVertexArrays(1, &ResourceID);
	}

	VertexArray::~VertexArray()
	{
		LOG("Destroyed vertexarray");
	}

	VertexArray::VertexArray(VertexArray& va) noexcept
		:m_VB(va.m_VB), m_IB(va.m_IB)
	{
		LOG("Copied vertexarray constructor");
		ResourceID = va.ResourceID;
	}

	VertexArray& VertexArray::operator=(VertexArray va) noexcept
	{
		LOG("Copied vertexarray operator");
		ResourceID = va.ResourceID;

		m_VB = va.m_VB;
		m_IB = va.m_IB;

		return *this;
	}

	void VertexArray::Free()
	{
		glDeleteVertexArrays(1, &ResourceID);
		
		OpenGLAPI::FreeBuffer(m_IB.ResourceID);
		OpenGLAPI::FreeBuffer(m_VB.ResourceID);
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
		OpenGLAPI::BindBuffer(m_VB.BufferType, m_VB.ResourceID);

		const auto& elements = m_VB.Layout.Elements;
		unsigned int offset = 0;
		for (unsigned int i = 0; i < elements.size(); i++) {
			const auto& element = elements[i];
			glVertexAttribPointer(i, element.Count, ShaderDataTypeToOpenGLBaseType(element.ShaderType), element.Normalized,
				m_VB.Layout.Stride, (void*)(size_t)offset);
			offset += element.Count * ShaderDataTypeSize(element.ShaderType);
			glEnableVertexAttribArray(i);
		}
	}

	void VertexArray::SetIndices(IndexBuffer& ib)
	{
		m_IB = ib;

		OpenGLAPI::BindBuffer(m_IB.BufferType, m_IB.ResourceID);
	}
}