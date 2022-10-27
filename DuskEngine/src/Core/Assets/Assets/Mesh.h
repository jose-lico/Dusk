#pragma once
#include "Core/Assets/Asset.h"

#include "Utils/Memory/Memory.h"
#include "Platform/OpenGL/VertexArray.h"

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

	class Mesh : public Asset
	{
	public:
		Mesh(float* vertices, unsigned int size, unsigned int* indices, unsigned int count, MeshType type);
		Mesh(std::vector<Vertex>& vertices, unsigned int* indices, unsigned int count);
		/*Mesh(const Mesh& mesh)
		{
			m_Name = mesh.m_Name;
			m_UUID = mesh.m_UUID;
			m_Path = mesh.m_Path;

			m_Type = mesh.m_Type;
			m_VA = mesh.m_VA;
		};*/

		~Mesh();

		inline MeshType GetType() { return m_Type; }
		inline const VertexArray& GetVertexArray() const { return m_VA; }
	private:
		VertexArray m_VA;
		MeshType m_Type;

		friend class AssetDatabase;
	};
}