#include "Core/ECS/Scene.h"
#include "Utils/Memory/Memory.h"

#include <unordered_map>
#include <filesystem>

namespace uuids
{
	class uuid;
}

namespace DuskEngine
{
	class uuids::uuid;

	class Material;
	class Shader;
	class Texture;
	class Mesh;

	class ResourceManager
	{
	public:
		static void Init();

		static void LoadUUIDs();
		static void CreateUUIDs();
		static void DeleteUUIDs();
		
		static uuids::uuid GetUUID(const std::filesystem::path& path);

		static Ref<Material> LoadMaterial(const uuids::uuid& uuid);
		static Ref<Shader> LoadShader(const uuids::uuid& uuid);
		static Ref<Texture> LoadTexture(const uuids::uuid& uuid);
		static Ref<Mesh> LoadModel(const uuids::uuid& uuid);
	private:
		static std::filesystem::path m_RootDirectory;
		static std::filesystem::path m_CurrentDirectory;

		static std::unordered_map <uuids::uuid, std::filesystem::path> m_UUIDsMap;
	};
}