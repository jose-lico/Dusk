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
	struct Texture;
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

		Material LoadMaterial(const uuids::uuid& uuid, AssetHandler* handler);
		Shader LoadShader(const uuids::uuid& uuid);
		Texture LoadTexture(const uuids::uuid& uuid);
		Mesh LoadModel(const uuids::uuid& uuid);

		UniqueRef<LuaScript> LoadScript(const uuids::uuid& uuid);

	private:
		std::filesystem::path m_RootDirectory = "res";
		std::filesystem::path m_CurrentDirectory;

		std::unordered_map <uuids::uuid, std::filesystem::path, std::hash<uuids::uuid>> m_PathsMap;
		std::unordered_map <std::filesystem::path, uuids::uuid, opt_path_hash> m_UUIDsMap;

		friend class WindowsWindow;
		friend class AssetDatabaseEditor;
	};
}