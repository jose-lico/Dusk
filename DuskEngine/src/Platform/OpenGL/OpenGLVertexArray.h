#pragma once

#include "Core/Renderer/VertexArray.h"
#include "OpenGLVertexBuffer.h"
#include "OpenGLIndexBuffer.h"

namespace DuskEngine
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();
		
		void Bind() const override;
		void Unbind() const override;

		void AddBuffer(const std::shared_ptr<VertexBuffer>& vb) override;
		void AddIndices(const std::shared_ptr<IndexBuffer>& ib) override;
		const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }
	private:
		unsigned int m_ID;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
	};
}