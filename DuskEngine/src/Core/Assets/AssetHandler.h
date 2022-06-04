#pragma once

#include "Utils/Memory/Memory.h"

#include "Assets/Shader.h"

#include "uuid.h"

#include <unordered_map>
#include <vector>

namespace DuskEngine
{
	template<typename AssetType>
	class AssetPool
	{
	public:
		AssetPool() = default;
		~AssetPool() = default;

		uint32_t AddToPool(std::unordered_map<uuids::uuid, uint32_t>& handleMap, const uuids::uuid& uuid)
		{
			if (handleMap.find(uuid) == handleMap.end())
			{
				// also need to make database templated somehow
				m_Pool.push_back(AssetDatabase::LoadShader(uuid));
				handleMap[uuid] = m_Pool.size() - 1;
				return m_Pool.size() - 1;
			}

			return handleMap[uuid];
		};

		Ref<AssetType>& operator()(uint32_t handle)
		{
			return m_Pool[handle];
		}
	private:
		std::vector<Ref<AssetType>> m_Pool;
	};

	class Material;
	class Texture;
	class Mesh;
	class Shader;
	class LuaScript;

	class AssetHandler
	{
	public:
		AssetHandler(const std::string& name);
		~AssetHandler();

		Ref<Mesh>& MeshPool(const uuids::uuid& uuid);
		void AddToMeshPool(const uuids::uuid& uuid);
		void AddToMeshPool(const uuids::uuid& uuid, Ref<Mesh>& mesh);

		Ref<Texture>& TexturePool(const uuids::uuid& uuid);
		void AddToTexturePool(const uuids::uuid& uuid);

		Ref<LuaScript>& LuaScriptPool(const uint32_t handle);
		uint32_t AddToLuaScriptPool(const uuids::uuid& uuid, const uuids::uuid& scriptUUID);

		Ref<Shader>& ShaderPool(const uuids::uuid& uuid);
		void AddToShaderPool(const uuids::uuid& uuid);

		Ref<Material>& MaterialPool(const uuids::uuid& uuid);
		void AddToMaterialPool(const uuids::uuid& uuid);
		static void PropagateMaterialChange(Material* material);

		Ref<Shader>& ShaderPool2(const uint32_t handle);
		uint32_t AddToShaderPool2(const uuids::uuid& uuid);
	private:
		std::string m_Name;
		std::unordered_map <uuids::uuid, Ref<Mesh>> m_MeshPool;
		std::unordered_map <uuids::uuid, Ref<Texture>> m_TexturePool;
		std::unordered_map <uuids::uuid, Ref<Material>> m_MaterialPool;
		std::unordered_map <uuids::uuid, Ref<Shader>> m_ShaderPool;
		
		std::vector<Ref<LuaScript>> m_LuaScriptPool;
		std::unordered_map<uuids::uuid, uint32_t> m_HandleMap;

		static std::vector<AssetHandler*> m_AssetHandlers;

		AssetPool<Shader> m_ShaderPool2;
	};
}