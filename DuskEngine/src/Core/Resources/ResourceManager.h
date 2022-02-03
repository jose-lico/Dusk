#include "Core/ECS/Scene.h"
#include "Utils/Memory/Memory.h"

#include "Core/Renderer/Resources/Material.h"
#include "Core/Renderer/Resources/Model.h"

#include <uuid.h>

#include <unordered_map>
#include <filesystem>

namespace DuskEngine
{
	class ResourceManager
	{
	public:
		static void Init();

		static void LoadUUIDs();
		static void CreateUUIDs();
		static void DeleteUUIDs();
		
		static std::string GetUUID(const std::string& path);

		static Ref<Material> LoadMaterial(const std::string& uuid);
		static Ref<Shader> LoadShader(const std::string& uuid);
		static Ref<Texture> LoadTexture(const std::string& uuid);
		static Ref<Mesh> LoadModel(const std::string& uuid);
	private:
		//static void CreateUUID();
		static std::filesystem::path m_RootDirectory;
		static std::filesystem::path m_CurrentDirectory;

		static std::unordered_map<std::string, std::string> m_UUIDsMap;
	};
}