#pragma once

#include "Buffer.h"

namespace DuskEngine
{
	class VertexArray : public Resource
	{
	public:
		VertexArray();
		~VertexArray() = default;

		VertexArray(VertexArray& va) noexcept;
		VertexArray& operator=(VertexArray va) noexcept;
				
		virtual void Free() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		void SetBuffer(VertexBuffer& vb);
		void SetIndices(IndexBuffer& ib);

		inline const IndexBuffer& GetIndexBuffer() const { return m_IB; }
	private:
		VertexBuffer m_VB;
		IndexBuffer m_IB;
	};
}