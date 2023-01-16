#pragma once
#include "Core/Assets/Asset.h"

#include "Utils/Memory/Memory.h"
#include "Mesh.h"

#include <vector>

namespace DuskEngine
{
	struct ModelData
	{
		uint32_t VertCount = 0;
		uint32_t IndicesSize = 0;
		uint32_t IndicesTypeSize = 0;
		uint32_t CompressedSize = 0;
	};

	class Mesh;
	
	class Model : public Asset
	{
	public:
		Model(const std::filesystem::path& path, const uuids::uuid& uuid);
		~Model();

		UniqueRef<Mesh>& GetFirstMesh() { return m_Meshes[0]; } // TODO -> temporary fix while Model/Mesh api isnt better
	private:
		std::vector<UniqueRef<Mesh>> m_Meshes;
	};
}