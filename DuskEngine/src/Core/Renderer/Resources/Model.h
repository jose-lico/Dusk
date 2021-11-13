#pragma once

#include "Mesh.h"

#include <assimp/scene.h>

#include <string>
#include <vector>

namespace DuskEngine
{
	class Model
	{
	public:
		Model(const std::string& path);
		~Model();
		std::vector<Ref<Mesh>> m_Meshes;
	private:
		void ProcessNode(aiNode* node, const aiScene* scene);
		Ref<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
	};
}