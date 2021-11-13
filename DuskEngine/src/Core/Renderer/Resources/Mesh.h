#pragma once
#include "Core/Macros/DUSK_API.h"

#include "Buffer.h"
#include "VertexArray.h"

#include <memory>
#include <glm/glm.hpp>

namespace DuskEngine
{
	struct Vertex {
		glm::vec3 Position;
		glm::vec2 TexCoords;
		glm::vec3 Normal;
	};

	class DUSK_EXPORT Mesh
	{
	public:
		Mesh(float* vertices, unsigned int size, unsigned int* indices, unsigned int count);
		Mesh(std::vector<Vertex>& vertices, unsigned int* indices, unsigned int count);
		~Mesh();
		std::shared_ptr<VertexArray> m_VA;
	};
}