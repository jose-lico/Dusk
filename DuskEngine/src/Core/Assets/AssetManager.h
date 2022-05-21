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

	class AssetManager
	{
	public:
		static void Init();
		static void Shutdown();

		static void LoadUUIDs();
		static void CreateUUIDs();
		static uuids::uuid CreateResource(const std::filesystem::path& path);
		static void DeleteUUIDs();
		
		static uuids::uuid GetUUID(const std::filesystem::path& path);

		static Ref<Material> LoadMaterial(const uuids::uuid& uuid);
		static Ref<Shader> LoadShader(const uuids::uuid& uuid);
		static Ref<Texture> LoadTexture(const uuids::uuid& uuid);
		static Ref<Mesh> LoadModel(const uuids::uuid& uuid);
		static Ref<LuaScript> LoadScript(const uuids::uuid& uuid);

		// Resource Lists
		static std::vector<Asset*> ShaderList;
		static std::vector<Asset*> ModelList;
		static std::vector<Asset*> MaterialList;
		static std::vector<Asset*> ScriptsList;
	private:
		static std::filesystem::path m_RootDirectory;
		static std::filesystem::path m_CurrentDirectory;

		static std::unordered_map <uuids::uuid, std::filesystem::path> m_PathsMap;
		static std::unordered_map <std::filesystem::path, uuids::uuid, opt_path_hash> m_UUIDsMap;

		static void AddToResourceList(const std::filesystem::path& path, const uuids::uuid& uuid);

		friend class WindowsWindow;
	};
}