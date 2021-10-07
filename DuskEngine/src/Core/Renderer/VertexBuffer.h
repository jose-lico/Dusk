#pragma once

#include "Core/Renderer/VertexBufferLayout.h"
#include "Core/Application/Macros.h"

namespace DuskEngine
{
	class DUSK_API VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const VertexBufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const VertexBufferLayout& layout) = 0;

		static VertexBuffer* Create(const void* data, int size);
	};
}