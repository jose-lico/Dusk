#pragma once
#include "Core/Renderer/Resources/Buffer.h"

namespace DuskEngine
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(const void* data, unsigned int size);
		~OpenGLVertexBuffer();

		void Bind() const override;
		void Unbind() const override;

		virtual const Ref<VertexBufferLayout>& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const Ref<VertexBufferLayout>& layout) override { m_Layout = layout; }
	private:
		unsigned int m_ID;
		Ref<VertexBufferLayout> m_Layout;
	};
}

