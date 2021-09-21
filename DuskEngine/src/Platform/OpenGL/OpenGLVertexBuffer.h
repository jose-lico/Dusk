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
	private:
		unsigned int m_ID;
	};
}

