#pragma once

#include <filesystem>
#include <unordered_map>
//#include <optional>
//
//struct opt_path_hash {
//	std::size_t operator()(const std::optional<std::filesystem::path>& path) const {
//		return path ? hash_value(path.value()) : 0;
//	}
//};

namespace uuids
{
	class uuid;
}

namespace DuskEngine
{
	class AssetDatabase;

	class AssetDatabaseEditor
	{
	public:
		AssetDatabaseEditor(AssetDatabase* database);
		~AssetDatabaseEditor();

		void RegisterAssets();
		void ImportAssets();
	private:
		void CreateMetaFile(const std::filesystem::directory_entry& directoryEntry);

		AssetDatabase* m_EngineDB;

		std::filesystem::path m_RootDirectory = "res";
		std::filesystem::path m_CurrentDirectory;
		std::unordered_map <std::filesystem::path, uuids::uuid> m_UUIDsMap;

		friend class AssetDatabase;
	};
}