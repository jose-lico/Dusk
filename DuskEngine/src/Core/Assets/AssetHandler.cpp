#include "pch.h"
#include "AssetHandler.h"

#include "AssetManager.h"

#include "Assets/Mesh.h"
#include "Assets/Texture.h"
#include "Assets/Material.h"

#include "Utils/Rendering/PrimitiveMesh.h"

namespace DuskEngine
{
	std::vector<AssetHandler*> AssetHandler::m_AssetHandlers;

	AssetHandler::AssetHandler(const std::string& name)
		:m_Name(name)
	{
		m_AssetHandlers.push_back(this);

		//  default texture every handler should have
		AddToTexturePool(AssetManager::GetUUID("res/textures/white.png")); 

		//  default meshes
		std::string str = "47183823-2574-4bfd-b411-99ed177d3e43";
		uuids::uuid id = uuids::uuid::from_string(str).value();
		auto quad = PrimitiveMesh::Quad();
		AddToMeshPool(id, quad);

		str = "47183823-2574-4bfd-b411-99ed177d3e44";
		id = uuids::uuid::from_string(str).value();
		auto cube = PrimitiveMesh::Cube();
		AddToMeshPool(id, cube);

		AddToMaterialPool(AssetManager::GetUUID("res/editor/materials/defaultMaterial.material"));
	}

	AssetHandler::~AssetHandler()
	{
		LOG(("Destroying Asset Handler " + m_Name).c_str());
	}

	Ref<Mesh>& AssetHandler::MeshPool(const uuids::uuid& uuid)
	{
		return m_MeshPool[uuid];
	}

	void AssetHandler::AddToMeshPool(const uuids::uuid& uuid)
	{
		if (m_MeshPool.find(uuid) == m_MeshPool.end())
			m_MeshPool.insert({ uuid, AssetManager::LoadModel(uuid) });
	}

	void AssetHandler::AddToMeshPool(const uuids::uuid& uuid, Ref<Mesh>& mesh)
	{
		if (m_MeshPool.find(uuid) == m_MeshPool.end())
			m_MeshPool.insert({ uuid, mesh });
	}

	Ref<Texture>& AssetHandler::TexturePool(const uuids::uuid& uuid)
	{
		return m_TexturePool[uuid];
	}

	void AssetHandler::AddToTexturePool(const uuids::uuid& uuid)
	{
		if (m_TexturePool.find(uuid) == m_TexturePool.end())
			m_TexturePool.insert({ uuid, AssetManager::LoadTexture(uuid) });
	}

	Ref<Shader>& AssetHandler::ShaderPool(const uuids::uuid& uuid)
	{
		return m_ShaderPool[uuid];
	}

	void AssetHandler::AddToShaderPool(const uuids::uuid& uuid)
	{
		if (m_ShaderPool.find(uuid) == m_ShaderPool.end())
			m_ShaderPool.insert({ uuid, AssetManager::LoadShader(uuid) });
	}

	Ref<Material>& AssetHandler::MaterialPool(const uuids::uuid& uuid)
	{
		return m_MaterialPool[uuid];
	}

	void AssetHandler::AddToMaterialPool(const uuids::uuid& uuid)
	{
		if (m_MaterialPool.find(uuid) == m_MaterialPool.end())
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
