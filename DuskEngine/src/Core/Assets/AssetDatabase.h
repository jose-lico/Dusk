#pragma once

#include "Utils/Memory/Memory.h"

#include "uuid.h"

#include <unordered_map>
#include <filesystem>
#include <optional>

#undef CreateMetaFile

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
	class AssetHandler;

	struct opt_path_hash {
		std::size_t operator()(const std::optional<std::filesystem::path>& path) const {
			return path ? hash_value(path.value()) : 0;
		}
	};

	class AssetDatabase
	{
	public:
		AssetDatabase();
		~AssetDatabase();

		void LoadProject();
		uuids::uuid CreateResource(const std::filesystem::path& path);
		
		uuids::uuid GetUUID(const std::filesystem::path& path);

		UniqueRef<Material> LoadMaterial(const uuids::uuid& uuid, AssetHandler* handler);
		UniqueRef<Shader> LoadShader(const uuids::uuid& uuid);
		UniqueRef<Texture> LoadTexture(const uuids::uuid& uuid);
		UniqueRef<Mesh> LoadModel(const uuids::uuid& uuid);
		UniqueRef<LuaScript> LoadScript(const uuids::uuid& uuid);

		// Asset Lists
		std::vector<Asset*> ShaderDatabase;
		std::vector<Asset*> ModelDatabase;
		std::vector<Asset*> MaterialDatabase;
		std::vector<Asset*> ScriptsDatabase;
	private:
		std::filesystem::path m_RootDirectory = "res";
		std::filesystem::path m_CurrentDirectory;

		std::unordered_map <uuids::uuid, std::filesystem::path, std::hash<uuids::uuid>> m_PathsMap;
		std::unordered_map <std::filesystem::path, uuids::uuid, opt_path_hash> m_UUIDsMap;

		void AddToAssetDatabase(const std::filesystem::path& path, const uuids::uuid& uuid);
		void CreateMetaFile(const std::filesystem::directory_entry& directoryEntry);
		void RegisterAsset(const std::filesystem::directory_entry& directoryEntry);

		friend class WindowsWindow;
	};
}