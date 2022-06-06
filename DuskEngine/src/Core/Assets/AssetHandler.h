#pragma once

#include "Utils/Memory/Memory.h"

#include "Assets/Shader.h"
#include "Handle.h"
#include "AssetPool.h"

#include "uuid.h"

#include <unordered_map>
#include <vector>

namespace DuskEngine
{
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
				return m_ShaderPool(handle);
			else if constexpr (std::is_same<T, Texture>::value)
				return m_TexturePool(handle);
			else if constexpr (std::is_same<T, Mesh>::value)
				return m_MeshPool(handle);
			else if constexpr (std::is_same<T, Material>::value)
				return m_MaterialPool(handle);
		}

		template<typename T>
		Handle<T> AddToAssetPool(const uuids::uuid& uuid)
		{
			Handle<T> handle;
			if constexpr (std::is_same<T, Shader>::value)
				return handle.m_Value = m_ShaderPool.AddToPool(m_HandleMap, uuid);
			else if constexpr (std::is_same<T, Texture>::value)
				return handle.m_Value = m_TexturePool.AddToPool(m_HandleMap, uuid);
			else if constexpr (std::is_same<T, Mesh>::value)
				return handle.m_Value = m_MeshPool.AddToPool(m_HandleMap, uuid);
			else if constexpr (std::is_same<T, Material>::value)
				return handle.m_Value = m_MaterialPool.AddToPool(m_HandleMap, uuid);
		}
	private:
		std::string m_Name;

		std::unordered_map<uuids::uuid, uint32_t> m_HandleMap;

		_AssetPool<Shader> m_ShaderPool;
		_AssetPool<Texture> m_TexturePool;
		_AssetPool<Material> m_MaterialPool;
		_AssetPool<Mesh> m_MeshPool;
		
		std::vector<Ref<LuaScript>> m_LuaScriptPool;
		
		static std::vector<AssetHandler*> m_AssetHandlers;
	};
}