#pragma once
#include "Core/Macros/DUSK_API.h"

namespace DuskEngine
{
	class DUSK_API IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual unsigned int GetCount() const = 0;

		static IndexBuffer* Create(const void* data, int count);
	};
}