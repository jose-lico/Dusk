#pragma once

#include "Buffer.h"

namespace DuskEngine
{
	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray() = default;
				
		void Free();

		void Bind() const;
		void Unbind() const;

		void SetBuffer(VertexBuffer& vb);
		void SetIndices(IndexBuffer& ib);

		inline const IndexBuffer& GetIndexBuffer() const { return m_IB; }
	private:
		uint32_t m_ResourceID;

		VertexBuffer m_VB;
		IndexBuffer m_IB;
	};
}