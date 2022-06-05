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
		_AssetPool(AssetHandler* assetHandler)
			:m_AssetHandler(assetHandler)
		{
			if constexpr (std::is_same<T, Mesh>::value)
			{
				m_Pool.push_back(PrimitiveMesh::Quad());
				m_Pool.push_back(PrimitiveMesh::Cube());
			}
			else if constexpr (std::is_same<T, Material>::value)
				m_Pool.push_back(AssetDatabase::LoadMaterial(
					AssetDatabase::GetUUID("res/editor/materials/defaultMaterial.material"), m_AssetHandler));
			else if constexpr (std::is_same<T, Texture>::value)
				m_Pool.push_back(AssetDatabase::LoadTexture(AssetDatabase::GetUUID("res/textures/white.png")));
		};
		~_AssetPool() = default;

		uint32_t AddToPool(std::unordered_map<uuids::uuid, uint32_t>& handleMap, const uuids::uuid& uuid)
		{
			if (handleMap.find(uuid) == handleMap.end())
			{
				if constexpr (std::is_same<T, Shader>::value)
					m_Pool.push_back(AssetDatabase::LoadShader(uuid));
				else if constexpr (std::is_same<T, Texture>::value)
					m_Pool.push_back(AssetDatabase::LoadTexture(uuid));
				else if constexpr (std::is_same<T, Mesh>::value)
					m_Pool.push_back(AssetDatabase::LoadModel(uuid));
				else if constexpr (std::is_same<T, Material>::value)
					m_Pool.push_back(AssetDatabase::LoadMaterial(uuid, m_AssetHandler));

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

		AssetHandler* m_AssetHandler;
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

		Ref<LuaScript>& LuaScriptPool(const uint32_t handle);
		uint32_t AddToLuaScriptPool(const uuids::uuid& uuid, const uuids::uuid& scriptUUID);

		template<typename T>
		Ref<T>& AssetPool(const Handle<T> handle)
		{
			if constexpr (std::is_same<T, Shader>::value)
				return m_ShaderPool->operator()(handle);
			else if constexpr (std::is_same<T, Texture>::value)
				return m_TexturePool->operator()(handle);
			else if constexpr (std::is_same<T, Mesh>::value)
				return m_MeshPool->operator()(handle);
			else if constexpr (std::is_same<T, Material>::value)
				return m_MaterialPool->operator()(handle);
		}

		template<typename T>
		Handle<T> AddToAssetPool(const uuids::uuid& uuid)
		{
			Handle<T> handle;
			if constexpr (std::is_same<T, Shader>::value)
				return handle.m_Value = m_ShaderPool->AddToPool(m_HandleMap, uuid);
			else if constexpr (std::is_same<T, Texture>::value)
				return handle.m_Value = m_TexturePool->AddToPool(m_HandleMap, uuid);
			else if constexpr (std::is_same<T, Mesh>::value)
				return handle.m_Value = m_MeshPool->AddToPool(m_HandleMap, uuid);
			else if constexpr (std::is_same<T, Material>::value)
				return handle.m_Value = m_MaterialPool->AddToPool(m_HandleMap, uuid);
		}
	private:
		std::string m_Name;

		std::unordered_map<uuids::uuid, uint32_t> m_HandleMap;

		_AssetPool<Shader>* m_ShaderPool;
		_AssetPool<Texture>* m_TexturePool;
		_AssetPool<Mesh>* m_MeshPool;
		_AssetPool<Material>* m_MaterialPool;
		
		std::vector<Ref<LuaScript>> m_LuaScriptPool;
		
		static std::vector<AssetHandler*> m_AssetHandlers;
	};
}