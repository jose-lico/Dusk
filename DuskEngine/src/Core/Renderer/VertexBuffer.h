#pragma once
#include "Core/Macros/DUSK_API.h"

#include "Core/Renderer/VertexBufferLayout.h"

namespace DuskEngine
{
	class DUSK_API VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const std::shared_ptr<VertexBufferLayout>& GetLayout() const = 0;
		virtual void SetLayout(const std::shared_ptr<VertexBufferLayout>& layout) = 0;

		static VertexBuffer* Create(const void* data, int size);
	};
}