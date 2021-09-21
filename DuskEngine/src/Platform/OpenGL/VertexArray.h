#pragma once

#include "OpenGLVertexBuffer.h"
#include "VertexBufferLayout.h"
#include "OpenGLIndexBuffer.h"

namespace DuskEngine
{
	class VertexArray {
	public:
		unsigned int Indices;
		VertexArray();
		~VertexArray();

		void AddBuffer(const VertexBuffer& vb);
		void AddLayout(const VertexBufferLayout& layout);
		void AddIndices(const IndexBuffer& ib);
		void Bind() const;
	private:
		unsigned int m_ID;
	};
}