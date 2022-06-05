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

		m_ShaderPool = new _AssetPool<Shader>(this);
		m_MeshPool = new _AssetPool<Mesh>(this);
		m_TexturePool = new _AssetPool<Texture>(this);
		m_MaterialPool = new _AssetPool<Material>(this);

		LOG(("Created Asset Handler " + m_Name).c_str());
	}

	AssetHandler::~AssetHandler()
	{
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
