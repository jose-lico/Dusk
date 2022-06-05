#pragma once

#include "Utils/Memory/Memory.h"

#include "Assets/Shader.h"

#include "uuid.h"

#include <unordered_map>
#include <vector>

namespace DuskEngine
{
	template<typename T>
	class AssetPool
	{
	public:
		AssetPool() = default;
		~AssetPool() = default;

		uint32_t AddToPool(std::unordered_map<uuids::uuid, uint32_t>& handleMap, const uuids::uuid& uuid)
		{
			if (handleMap.find(uuid) == handleMap.end())
			{
				if constexpr (std::is_same<T, Shader>::value)
					m_Pool.push_back(AssetDatabase::LoadShader(uuid));
				else if constexpr (std::is_same<T, Texture>::value)
					m_Pool.push_back(AssetDatabase::LoadTexture(uuid));

				handleMap[uuid] = m_Pool.size() - 1;
				return m_Pool.size() - 1;
			}

			return handleMap[uuid];
		};

		Ref<T>& operator()(uint32_t handle)
		{
			return m_Pool[handle];
		}
	private:
		std::vector<Ref<T>> m_Pool;
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

		Ref<LuaScript>& LuaScriptPool(const uint32_t handle);
		uint32_t AddToLuaScriptPool(const uuids::uuid& uuid, const uuids::uuid& scriptUUID);

		Ref<Material>& MaterialPool(const uuids::uuid& uuid);
		void AddToMaterialPool(const uuids::uuid& uuid);
		static void PropagateMaterialChange(Material* material);

		Ref<Shader>& ShaderPool(const uint32_t handle);
		uint32_t AddToShaderPool(const uuids::uuid& uuid);

		Ref<Texture>& TexturePool(const uint32_t handle);
		uint32_t AddToTexturePool(const uuids::uuid& uuid);
	private:
		std::string m_Name;
		std::unordered_map <uuids::uuid, Ref<Mesh>> m_MeshPool;
		//std::unordered_map <uuids::uuid, Ref<Texture>> m_TexturePool;
		std::unordered_map <uuids::uuid, Ref<Material>> m_MaterialPool;
		
		std::vector<Ref<LuaScript>> m_LuaScriptPool;
		std::unordered_map<uuids::uuid, uint32_t> m_HandleMap;

		static std::vector<AssetHandler*> m_AssetHandlers;

		AssetPool<Shader> m_ShaderPool;
		AssetPool<Texture> m_TexturePool;
	};
}