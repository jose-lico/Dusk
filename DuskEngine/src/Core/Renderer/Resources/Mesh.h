#pragma once
#include "Core/Macros/DUSK_API.h"

#include "Buffer.h"
#include "VertexArray.h"

#include <memory>

namespace DuskEngine
{
	class DUSK_EXPORT Mesh
	{
	public:
		Mesh(float* vertices, unsigned int size, unsigned int* indices, unsigned int count);
		~Mesh();
		std::shared_ptr<VertexArray> m_VA;
	};
}