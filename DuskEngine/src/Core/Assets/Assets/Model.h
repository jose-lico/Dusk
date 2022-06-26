#pragma once
#include "Core/Assets/Asset.h"

#include "Utils/Memory/Memory.h"

#include <string>
#include <vector>

struct aiNode;
struct aiScene;
struct aiMesh;

namespace DuskEngine
{
	class Mesh;
	
	class Model : public Asset
	{
	public:
		//Model(const std::string& path); not used anymore, keeping just in case
		Model(const std::filesystem::path& path, const uuids::uuid& uuid);
		~Model();

		UniqueRef<Mesh>& GetFirstMesh() { return m_Meshes[0]; } // TODO -> temporary fix while Model/Mesh api isnt better
	private:
		std::vector<UniqueRef<Mesh>> m_Meshes;
		void ProcessNode(aiNode* node, const aiScene* scene);
		UniqueRef<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
	};
}