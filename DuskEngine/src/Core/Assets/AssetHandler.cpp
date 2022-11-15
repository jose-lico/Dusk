#include "pch.h"
#include "AssetHandler.h"

#include "Core/Application/Application.h"
#include "AssetDatabase.h"
#include "Handle.h"

#include "Assets/Mesh.h"
#include "Platform/OpenGL/OpenGLAPI.h"
#include "Platform/OpenGL/Texture.h"
#include "Assets/Material.h"
#include "Utils/Rendering/PrimitiveMesh.h"
#include "Core/Scripting/LuaScript.h"
#include "Utils/Profiling/Timer.h"

#include <iostream>

namespace DuskEngine
{
	std::vector<AssetHandler*> AssetHandler::m_AssetHandlers;

	AssetHandler::AssetHandler(const std::string& name)
		:m_Name(name)
	{
		Timer assetHandlerTimer("Asset Handler " + name);
		m_AssetHandlers.push_back(this);

		m_ShaderPool.m_AssetHandler = this;
		m_MeshPool.m_AssetHandler = this;
		m_TexturePool.m_AssetHandler = this;
		m_MaterialPool.m_AssetHandler = this;

		m_ShaderPool.m_Pool.reserve(10);
		m_MeshPool.m_Pool.reserve(10);
		m_TexturePool.m_Pool.reserve(10);
		m_MaterialPool.m_Pool.reserve(10);

		m_MeshPool.m_Pool.push_back(PrimitiveMesh::Quad());
		m_MeshPool.m_Pool.push_back(PrimitiveMesh::Cube());

		// Ultimately this material will be embedded in the engine
#ifdef DUSK_WINDOWS
		m_MaterialPool.m_Pool.push_back(Application::Get().GetAssetDatabase().LoadMaterial(
			Application::Get().GetAssetDatabase().GetUUID("C:/dev/Dusk/DuskEditor/res/editor/materials/defaultMaterial.material"), this));
#elif DUSK_LINUX
		m_MaterialPool.m_Pool.push_back(Application::Get().GetAssetDatabase().LoadMaterial(
			Application::Get().GetAssetDatabase().GetUUID("C:/dev/Dusk/DuskEditor/res/editor/materials/defaultMaterial.material"), this));
#endif

		LOG(("Created Asset Handler " + m_Name).c_str());
	}

	AssetHandler::~AssetHandler()
	{
		for(auto& mesh : m_MeshPool.m_Pool)
		{
			OpenGLAPI::FreeVertexArray(mesh.m_VA);
		}

		for (auto& texture : m_TexturePool.m_Pool)
		{
			OpenGLAPI::FreeTexture(texture);
		}

		for (auto& shader : m_ShaderPool.m_Pool)
		{
			OpenGLAPI::DeleteProgram(shader);
		}

		LOG(("Destroying Asset Handler " + m_Name).c_str());
	}

	UniqueRef<LuaScript>& AssetHandler::LuaScriptPool(const uint32_t handle)
	{
		return m_LuaScriptPool[handle];
	}

	uint32_t AssetHandler::AddToLuaScriptPool(const uuids::uuid& uuid, const uuids::uuid& scriptUUID)
	{
		if (m_HandleMap.find(uuid) == m_HandleMap.end())
		{
			m_LuaScriptPool.push_back(Application::Get().GetAssetDatabase().LoadScript(scriptUUID));
			m_HandleMap[uuid] = (uint32_t)m_LuaScriptPool.size();
			return (uint32_t)m_LuaScriptPool.size() - 1;
		}

		return m_HandleMap[uuid];
	}
}
