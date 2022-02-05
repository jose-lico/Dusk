#pragma once
#include "Core/Resources/Resource.h"
#include "Mesh.h"

#include <assimp/scene.h>

#include <string>
#include <vector>

namespace DuskEngine
{
	class Model : public Resource
	{
	public:
		Model(const std::string& path);
		Model(const std::filesystem::path& path, const uuids::uuid& uuid);
		~Model();

		Ref<Mesh> GetFirstMesh() { return m_Meshes[0]; } // TODO -> temporary fix while Model/Mesh api isnt better
	private:
		std::vector<Ref<Mesh>> m_Meshes;
		void ProcessNode(aiNode* node, const aiScene* scene);
		Ref<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
	};
}