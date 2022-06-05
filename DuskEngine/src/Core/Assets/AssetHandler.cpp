#include "pch.h"
#include "AssetHandler.h"

#include "AssetDatabase.h"
#include "Handle.h"

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
		AddToAssetPool<Texture>(AssetDatabase::GetUUID("res/textures/white.png"));

		//  default meshes
		std::string str = "47183823-2574-4bfd-b411-99ed177d3e43";
		uuids::uuid id = uuids::uuid::from_string(str).value();
		auto quad = PrimitiveMesh::Quad();
		AddToMeshPool(id, quad);

		str = "47183823-2574-4bfd-b411-99ed177d3e44";
		id = uuids::uuid::from_string(str).value();
		auto cube = PrimitiveMesh::Cube();
		AddToMeshPool(id, cube);

		AddToMaterialPool(AssetDatabase::GetUUID("res/editor/materials/defaultMaterial.material"));
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
			m_MeshPool.insert({ uuid, AssetDatabase::LoadModel(uuid) });
	}

	void AssetHandler::AddToMeshPool(const uuids::uuid& uuid, Ref<Mesh>& mesh)
	{
		if (m_MeshPool.find(uuid) == m_MeshPool.end())
			m_MeshPool.insert({ uuid, mesh });
	}



	Ref<LuaScript>& AssetHandler::LuaScriptPool(const uint32_t handle)
	{
		return m_LuaScriptPool[handle];
	}

	uint32_t AssetHandler::AddToLuaScriptPool(const uuids::uuid& uuid, const uuids::uuid& scriptUUID)
	{
		if (m_HandleMap.find(uuid) == m_HandleMap.end())
		{
			m_LuaScriptPool.push_back(AssetDatabase::LoadScript(scriptUUID));
			m_HandleMap[uuid] = m_LuaScriptPool.size();
			return m_LuaScriptPool.size() - 1;
		}

		return m_HandleMap[uuid];
	}

	Ref<Material>& AssetHandler::MaterialPool(const uuids::uuid& uuid)
	{
		return m_MaterialPool[uuid];
	}

	void AssetHandler::AddToMaterialPool(const uuids::uuid& uuid)
	{
		if (m_MaterialPool.find(uuid) == m_MaterialPool.end())
			m_MaterialPool.insert({ uuid, AssetDatabase::LoadMaterial(uuid, this) });
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
