#pragma once
#include "Core/Renderer/Resources/Buffer.h"

#include <memory>

namespace DuskEngine
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(const void* data, unsigned int size);
		~OpenGLVertexBuffer();

		void Bind() const override;
		void Unbind() const override;

		virtual const std::shared_ptr<VertexBufferLayout>& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const std::shared_ptr<VertexBufferLayout>& layout) override { m_Layout = layout; }
	private:
		unsigned int m_ID;
		std::shared_ptr<VertexBufferLayout> m_Layout;
	};
}

