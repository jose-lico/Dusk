#pragma once

#include "Assets/Mesh.h"

#include <unordered_map>

namespace DuskEngine
{
	class AssetHandler
	{
	public:
		Ref<Mesh>& MeshPool(const uuids::uuid& uuid);
		void AddToMeshPool(const uuids::uuid& uuid);
		void AddToMeshPool(const uuids::uuid& uuid, Ref<Mesh>& mesh);
	private:
		std::unordered_map <uuids::uuid, Ref<Mesh>> m_MeshPool;
	};
}