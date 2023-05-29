#pragma once

#include "Utils/Memory/Memory.h"

#include "Handle.h"
#include "AssetPool.h"

#include "uuid.h"

#include <unordered_map>
#include <vector>

namespace DuskEngine
{
	class Material;
	struct Texture;
	struct Shader;
	class Mesh;
	class LuaScript;

	class AssetHandler
	{
	public:
		AssetHandler(const std::string& name, const std::filesystem::path& projectPath);
		~AssetHandler();

		Handle<LuaScript> AddToLuaScriptPool(const uuids::uuid& uuid, const uuids::uuid& scriptUUID);

		template<typename T>
		T& GetAsset(const Handle<T> handle)
		{
			if constexpr (std::is_same<T, Shader>::value)
				return m_ShaderPool(handle);
			else if constexpr (std::is_same<T, Texture>::value)
				return m_TexturePool(handle);
			else if constexpr (std::is_same<T, Mesh>::value)
				return m_MeshPool(handle);
			else if constexpr (std::is_same<T, Material>::value)
				return m_MaterialPool(handle);
			else if constexpr (std::is_same<T, LuaScript>::value)
				return m_LuaScriptPool(handle);
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
		std::filesystem::path m_ProjectPath;

		std::unordered_map<uuids::uuid, uint32_t> m_HandleMap;

		AssetPool<Shader> m_ShaderPool;
		AssetPool<Texture> m_TexturePool;
		AssetPool<Material> m_MaterialPool;
		AssetPool<Mesh> m_MeshPool;
		AssetPool<LuaScript> m_LuaScriptPool;
				
		static std::vector<AssetHandler*> m_AssetHandlers;

		friend class AssetPool<Shader>;
		friend class AssetPool<Texture>;
		friend class AssetPool<Material>;
		friend class AssetPool<Mesh>;
		friend class AssetPool<LuaScript>;
	};
}