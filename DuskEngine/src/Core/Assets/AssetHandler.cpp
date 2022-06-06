#include "pch.h"
#include "AssetHandler.h"

#include "AssetDatabase.h"
#include "Handle.h"

#include "Assets/Mesh.h"
#include "Assets/Texture.h"
#include "Assets/Material.h"

#include "Utils/Rendering/PrimitiveMesh.h"

#include <iostream>

namespace DuskEngine
{
	std::vector<AssetHandler*> AssetHandler::m_AssetHandlers;

	AssetHandler::AssetHandler(const std::string& name)
		:m_Name(name)
	{
		m_AssetHandlers.push_back(this);

		m_ShaderPool.m_AssetHandler = this;
		m_MeshPool.m_AssetHandler = this;
		m_TexturePool.m_AssetHandler = this;
		m_MaterialPool.m_AssetHandler = this;

		m_MeshPool.m_Pool.push_back(PrimitiveMesh::Quad());
		m_MeshPool.m_Pool.push_back(PrimitiveMesh::Cube());

		m_MaterialPool.m_Pool.push_back(AssetDatabase::LoadMaterial(
			AssetDatabase::GetUUID("res/editor/materials/defaultMaterial.material"), this));

		LOG(("Created Asset Handler " + m_Name).c_str());
	}

	AssetHandler::~AssetHandler()
	{
		/*delete(m_ShaderPool);
		delete(m_MeshPool);
		delete(m_TexturePool);
		delete(m_MaterialPool);*/
		LOG(("Destroying Asset Handler " + m_Name).c_str());
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
}
