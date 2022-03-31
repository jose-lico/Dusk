#pragma once
#include "Core/Macros/DUSK_API.h"

#include "Core/Resources/Resource.h"

#include "Utils/Memory/Memory.h"

#include "glm/glm.hpp"

namespace DuskEngine
{
	class VertexArray;

	struct Vertex {
		glm::vec3 Position;
		glm::vec2 TexCoords;
		glm::vec3 Normal;
	};

	enum class MeshType
	{
		Quad,
		Cube,
		Model
	};

	class DUSK_EXPORT Mesh : public Resource
	{
	public:
		Mesh(float* vertices, unsigned int size, unsigned int* indices, unsigned int count, MeshType type);
		Mesh(std::vector<Vertex>& vertices, unsigned int* indices, unsigned int count);
		~Mesh();
		Ref<VertexArray> m_VA; // TODO remove
		MeshType GetType() { return m_Type; }
	private:
		MeshType m_Type;

		friend class ResourceManager;
	};
}