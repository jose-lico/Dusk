#include "pch.h"
#include "AssetHandler.h"

#include "AssetManager.h"

namespace DuskEngine
{
	Ref<Mesh>& AssetHandler::MeshPool(const uuids::uuid& uuid)
	{
		return m_MeshPool[uuid];
	}

	void AssetHandler::AddToMeshPool(const uuids::uuid& uuid)
	{
		m_MeshPool.insert({ uuid, AssetManager::LoadModel(uuid) });
	}

	void AssetHandler::AddToMeshPool(const uuids::uuid& uuid, Ref<Mesh>& mesh)
	{
		m_MeshPool.insert({ uuid, mesh });
	}
}
