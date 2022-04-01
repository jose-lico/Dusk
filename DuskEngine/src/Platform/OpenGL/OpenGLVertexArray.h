#pragma once
#include "Core/Renderer/Resources/VertexArray.h"

#include "Utils/Memory/Memory.h"

namespace DuskEngine
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();
		
		void Bind() const override;
		void Unbind() const override;

		void AddBuffer(const Ref<VertexBuffer>& vb) override;
		void AddIndices(const Ref<IndexBuffer>& ib) override;
		const Ref<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }
	private:
		unsigned int m_ID;
		Ref<IndexBuffer> m_IndexBuffer;
		Ref<VertexBuffer> m_VertexBuffer;
	};
}