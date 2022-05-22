#include "pch.h"
#include "AssetHandler.h"

#include "AssetManager.h"

#include "Assets/Mesh.h"
#include "Assets/Texture.h"
#include "Assets/Material.h"

namespace DuskEngine
{
	std::vector<AssetHandler*> AssetHandler::m_AssetHandlers;

	AssetHandler::AssetHandler(const std::string& name)
		:m_Name(name)
	{
		m_AssetHandlers.push_back(this);

		AddToTexturePool(AssetManager::GetUUID("res/textures/white.png")); //  default texture every handler should have
	}

	AssetHandler::~AssetHandler()
	{
		// should somehow deregister the asset handler from the ds, fuck it for now
	}

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

	Ref<Texture>& AssetHandler::TexturePool(const uuids::uuid& uuid)
	{
		return m_TexturePool[uuid];
	}

	void AssetHandler::AddToTexturePool(const uuids::uuid& uuid)
	{
		m_TexturePool.insert({ uuid, AssetManager::LoadTexture(uuid) });
	}

	Ref<Material>& AssetHandler::MaterialPool(const uuids::uuid& uuid)
	{
		return m_MaterialPool[uuid];
	}

	void AssetHandler::AddToMaterialPool(const uuids::uuid& uuid)
	{
		m_MaterialPool.insert({ uuid, AssetManager::LoadMaterial(uuid, this) });
	}

	void AssetHandler::PropagateMaterialChange(Material* material)
	{
		// could be optimized by skipping the propagating pool
		for(auto handler : m_AssetHandlers)
		{
			handler->m_MaterialPool[material->GetUUID()].reset(material);
		}
	}
}
