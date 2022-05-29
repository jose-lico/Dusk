#pragma once

#include "Utils/Memory/Memory.h"

#include "uuid.h"

#include <unordered_map>

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

		Ref<Mesh>& MeshPool(const uuids::uuid& uuid);
		void AddToMeshPool(const uuids::uuid& uuid);
		void AddToMeshPool(const uuids::uuid& uuid, Ref<Mesh>& mesh);

		Ref<Texture>& TexturePool(const uuids::uuid& uuid);
		void AddToTexturePool(const uuids::uuid& uuid);

		Ref<LuaScript>& LuaScriptPool(const uuids::uuid& uuid);
		void AddToLuaScriptPool(const uuids::uuid& uuid);

		Ref<Shader>& ShaderPool(const uuids::uuid& uuid);
		void AddToShaderPool(const uuids::uuid& uuid);

		Ref<Material>& MaterialPool(const uuids::uuid& uuid);
		void AddToMaterialPool(const uuids::uuid& uuid);
		static void PropagateMaterialChange(Material* material);
	private:
		std::string m_Name;
		std::unordered_map <uuids::uuid, Ref<Mesh>> m_MeshPool;
		std::unordered_map <uuids::uuid, Ref<Texture>> m_TexturePool;
		std::unordered_map <uuids::uuid, Ref<Material>> m_MaterialPool;
		std::unordered_map <uuids::uuid, Ref<Shader>> m_ShaderPool;
		
		std::unordered_map <uuids::uuid, Ref<LuaScript>> m_LuaScriptPool;

		static std::vector<AssetHandler*> m_AssetHandlers;
	};
}