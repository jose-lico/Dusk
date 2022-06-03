#include "pch.h"
#include "AssetHandler.h"

#include "AssetDatabase.h"

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
		AddToTexturePool(AssetDatabase::GetUUID("res/textures/white.png"));

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

	Ref<Texture>& AssetHandler::TexturePool(const uuids::uuid& uuid)
	{
		return m_TexturePool[uuid];
	}

	void AssetHandler::AddToTexturePool(const uuids::uuid& uuid)
	{
		if (m_TexturePool.find(uuid) == m_TexturePool.end())
			m_TexturePool.insert({ uuid, AssetDatabase::LoadTexture(uuid) });
	}

	Ref<LuaScript>& AssetHandler::LuaScriptPool(const uuids::uuid& uuid)
	{
		return m_LuaScriptPool[uuid];
	}

	void AssetHandler::AddToLuaScriptPool(const uuids::uuid& uuid, const uuids::uuid& scriptUUID)
	{
		if (m_LuaScriptPool.find(uuid) == m_LuaScriptPool.end())
			m_LuaScriptPool.insert({ uuid, AssetDatabase::LoadScript(scriptUUID) });
	}

	Ref<LuaScript>& AssetHandler::LuaScriptPool2(const uint32_t handle)
	{
		return m_LuaScriptPool2[handle];
	}

	uint32_t AssetHandler::AddToLuaScriptPool2(const uuids::uuid& uuid, const uuids::uuid& scriptUUID)
	{
		if (m_HandleMap.find(uuid) == m_HandleMap.end())
		{
			m_LuaScriptPool2.push_back(AssetDatabase::LoadScript(scriptUUID));
			m_HandleMap[uuid] = m_LuaScriptPool2.size();
			return m_LuaScriptPool2.size() - 1;
		}

		return m_HandleMap[uuid];
	}

	Ref<Shader>& AssetHandler::ShaderPool(const uuids::uuid& uuid)
	{
		return m_ShaderPool[uuid];
	}

	void AssetHandler::AddToShaderPool(const uuids::uuid& uuid)
	{
		if (m_ShaderPool.find(uuid) == m_ShaderPool.end())
			m_ShaderPool.insert({ uuid, AssetDatabase::LoadShader(uuid) });
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
