#pragma once

#include "Utils/Memory/Memory.h"

#include "Assets/Shader.h"
#include "Handle.h"

#include "uuid.h"

#include <unordered_map>
#include <vector>

namespace DuskEngine
{
	template<typename T>
	class _AssetPool
	{
	public:
		_AssetPool() = default;
		~_AssetPool() = default;

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

		template<typename T>
		Ref<T>& AssetPool(const Handle<T> handle)
		{
			if constexpr (std::is_same<T, Shader>::value)
				return m_ShaderPool(handle);
			else if constexpr (std::is_same<T, Texture>::value)
				return m_TexturePool(handle);
		}

		template<typename T>
		Handle<T> AddToAssetPool(const uuids::uuid& uuid)
		{
			Handle<T> handle;
			if constexpr (std::is_same<T, Shader>::value)
			{
				return handle.m_Value = m_ShaderPool.AddToPool(m_HandleMap, uuid);
			}
			else if constexpr (std::is_same<T, Texture>::value)
			{
				return handle.m_Value = m_TexturePool.AddToPool(m_HandleMap, uuid);
			}
		}
	private:
		std::string m_Name;

		std::unordered_map<uuids::uuid, uint32_t> m_HandleMap;

		_AssetPool<Shader> m_ShaderPool;
		_AssetPool<Texture> m_TexturePool;
		std::unordered_map <uuids::uuid, Ref<Mesh>> m_MeshPool; // to be replaced
		std::unordered_map <uuids::uuid, Ref<Material>> m_MaterialPool; // to be replaced
		
		std::vector<Ref<LuaScript>> m_LuaScriptPool;
		
		static std::vector<AssetHandler*> m_AssetHandlers;
	};
}