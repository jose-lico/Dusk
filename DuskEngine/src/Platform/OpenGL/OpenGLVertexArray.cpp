#include "pch.h"
#include "OpenGLVertexArray.h"
#include "GLCommon.h"

#include "OpenGLVertexBuffer.h"
#include "OpenGLIndexBuffer.h"

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


	OpenGLVertexArray::OpenGLVertexArray()
	{
		glGenVertexArrays(1, &m_ID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_ID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_ID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddIndices(const Ref<IndexBuffer>& ib)
	{
		ib->Bind();
		m_IndexBuffer = ib;
	}

	void OpenGLVertexArray::AddBuffer(const Ref<VertexBuffer>& vb)
	{
		vb->Bind();
		m_VertexBuffer = vb;

		const auto& elements = vb->GetLayout()->GetElements();
		unsigned int offset = 0;
		for (unsigned int i = 0; i < elements.size(); i++) {
			const auto& element = elements[i];
			glVertexAttribPointer(i, element.count, ShaderDataTypeToOpenGLBaseType(element.type), element.normalized, vb->GetLayout()->GetStride(), (const void*)offset);
			offset += element.count * ShaderDataTypeSize(element.type);
			glEnableVertexAttribArray(i);
		}
	}
}