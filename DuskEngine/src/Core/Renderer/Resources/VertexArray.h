#pragma once
#include "Core/Application/Core.h"

namespace DuskEngine
{
	class VertexBuffer;
	class IndexBuffer;

	class DUSK_EXPORT VertexArray
	{
	public:
		virtual ~VertexArray() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddBuffer(const std::shared_ptr<VertexBuffer>& vb) = 0;
		virtual void AddIndices(const std::shared_ptr<IndexBuffer>& ib) = 0;
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;

		static VertexArray* Create();
	};
}