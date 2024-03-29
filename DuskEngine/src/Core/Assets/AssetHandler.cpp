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
#include "Platform/OpenGL/Shader.h"

#include "shaders/phong.glsl.vert.embedded"
#include "shaders/phong.glsl.frag.embedded"

#include <iostream>

namespace DuskEngine
{
	std::vector<AssetHandler*> AssetHandler::m_AssetHandlers;

	AssetHandler::AssetHandler(const std::string& name, const std::filesystem::path& projectPath)
		:m_Name(name), m_ProjectPath(projectPath)
	{
		ScopeTimer assetHandlerTimer("Asset Handler " + name);
		m_AssetHandlers.push_back(this);

		m_ShaderPool.m_AssetHandler = this;
		m_MeshPool.m_AssetHandler = this;
		m_TexturePool.m_AssetHandler = this;
		m_MaterialPool.m_AssetHandler = this;

		m_ShaderPool.m_Pool.reserve(10);
		m_MeshPool.m_Pool.reserve(10);
		m_TexturePool.m_Pool.reserve(10);
		m_MaterialPool.m_Pool.reserve(10);
		m_LuaScriptPool.m_Pool.reserve(10);

		m_MeshPool.m_Pool.push_back(PrimitiveMesh::Quad());
		m_MeshPool.m_Pool.push_back(PrimitiveMesh::Cube());
		
		m_ShaderPool.m_Pool.push_back(CreateShader((const char*)EMBEDDED_PHONGVERT, (const char*)EMBEDDED_PHONGFRAG, "Default Phong Shader"));

		m_MaterialPool.m_Pool.push_back(Material::CreateDefaultMaterial(this));

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

	Handle<LuaScript> AssetHandler::AddToLuaScriptPool(const uuids::uuid& uuid, const uuids::uuid& scriptUUID)
	{
		if (m_HandleMap.find(uuid) == m_HandleMap.end())
		{
			m_LuaScriptPool.m_Pool.push_back(Application::Get().GetAssetDatabase().LoadScript(scriptUUID));
			m_HandleMap[uuid] = (uint32_t)m_LuaScriptPool.m_Pool.size() -1;
			return (uint32_t)m_LuaScriptPool.m_Pool.size() - 1;
		}

		return m_HandleMap[uuid];
	}
}
