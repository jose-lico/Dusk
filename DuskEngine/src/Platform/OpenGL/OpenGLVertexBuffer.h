#pragma once

#include "Core/Renderer/VertexBuffer.h"

namespace DuskEngine
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(const void* data, unsigned int size);
		~OpenGLVertexBuffer();

		void Bind() const override;
		void Unbind() const override;

		virtual const VertexBufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const VertexBufferLayout& layout) override { m_Layout = layout; }
	private:
		unsigned int m_ID;
		VertexBufferLayout m_Layout;
	};
}

