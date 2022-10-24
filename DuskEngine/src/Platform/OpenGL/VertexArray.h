#pragma once

#include "Utils/Memory/Memory.h"

namespace DuskEngine
{
	class VertexBuffer;
	class IndexBuffer;

	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		VertexArray(VertexArray&& va);
		VertexArray& operator=(VertexArray&& va);
				
		void Bind() const;
		void Unbind() const;

		void AddBuffer(UniqueRef<VertexBuffer>& vb);
		void AddIndices(UniqueRef<IndexBuffer>& ib);
		const UniqueRef<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }
	private:
		unsigned int m_ID;
		UniqueRef<VertexBuffer> m_VertexBuffer;
		UniqueRef<IndexBuffer> m_IndexBuffer;
	};
}