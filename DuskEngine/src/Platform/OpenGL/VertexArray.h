#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"

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