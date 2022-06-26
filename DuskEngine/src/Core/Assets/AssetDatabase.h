#pragma once

#include "Core/ECS/Scene.h"

#include "Utils/Memory/Memory.h"

#include <unordered_map>
#include <filesystem>
#include <optional>

namespace uuids
{
	class uuid;
}

namespace DuskEngine
{
	class Asset;
	class Material;
	class Shader;
	class Texture;
	class Mesh;
	class LuaScript;

	struct opt_path_hash {
		std::size_t operator()(const std::optional<std::filesystem::path>& path) const {
			return path ? hash_value(path.value()) : 0;
		}
	};

	class AssetDatabase
	{
	public:
		static void Init();
		static void Shutdown();

		static void LoadUUIDs();
		static void CreateUUIDs();
		static uuids::uuid CreateResource(const std::filesystem::path& path);
		static void DeleteUUIDs();
		
		static uuids::uuid GetUUID(const std::filesystem::path& path);

		static UniqueRef<Material> LoadMaterial(const uuids::uuid& uuid, AssetHandler* handler);
		static UniqueRef<Shader> LoadShader(const uuids::uuid& uuid);
		static UniqueRef<Texture> LoadTexture(const uuids::uuid& uuid);
		static UniqueRef<Mesh> LoadModel(const uuids::uuid& uuid);
		static UniqueRef<LuaScript> LoadScript(const uuids::uuid& uuid);

		// Asset Lists
		static std::vector<Asset*> ShaderDatabase;
		static std::vector<Asset*> ModelDatabase;
		static std::vector<Asset*> MaterialDatabase;
		static std::vector<Asset*> ScriptsDatabase;
	private:
		static std::filesystem::path m_RootDirectory;
		static std::filesystem::path m_CurrentDirectory;

		static std::unordered_map <uuids::uuid, std::filesystem::path> m_PathsMap;
		static std::unordered_map <std::filesystem::path, uuids::uuid, opt_path_hash> m_UUIDsMap;

		static void AddToAssetDatabase(const std::filesystem::path& path, const uuids::uuid& uuid);

		friend class WindowsWindow;
	};
}