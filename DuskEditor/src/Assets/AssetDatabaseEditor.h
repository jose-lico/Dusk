#pragma once

#include "uuid.h"

#include <vector>
#include <filesystem>
#include <unordered_map>
#include <optional>

#undef CreateMetaFile

namespace uuids
{
	class uuid;
}

namespace DuskEngine
{
	class AssetDatabase;
	class Asset;

	struct opt_path_hash2 {
		std::size_t operator()(const std::optional<std::filesystem::path>& path) const {
			return path ? hash_value(path.value()) : 0;
		}
	};


	class AssetDatabaseEditor
	{
	public:
		AssetDatabaseEditor(AssetDatabase* database);
		~AssetDatabaseEditor();

		void RegisterAssets();
		void ImportAssets();
	private:
		void RegisterAsset(const std::filesystem::directory_entry& directoryEntry);
		void CreateMetaFile(const std::filesystem::directory_entry& directoryEntry);
		void AddToAssetDatabase(const std::filesystem::path& path, const uuids::uuid& uuid);

		AssetDatabase* m_EngineDB;

		std::filesystem::path m_RootDirectory = "res";
		std::filesystem::path m_CurrentDirectory;
		std::unordered_map <std::filesystem::path, uuids::uuid, opt_path_hash2> m_UUIDsMap;

		std::vector<std::tuple<std::filesystem::path, std::filesystem::path>> m_ImagesToImport;

		// Asset Lists
		std::vector<Asset*> m_ShaderDatabase;
		std::vector<Asset*> m_ModelDatabase;
		std::vector<Asset*> m_MaterialDatabase;
		std::vector<Asset*> m_ScriptsDatabase;

		friend class AssetDatabase;
		friend class InspectorPanel;
	};
}