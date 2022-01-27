#include "Core/ECS/Scene.h"
#include "Utils/Memory/Memory.h"

#include <uuid.h>

#include <unordered_map>
#include <filesystem>

#include "Core/Renderer/Resources/Material.h"

namespace DuskEngine
{
	class ResourceManager
	{
	public:
		static void CreateUUIDs();
		static void LoadUUIDs();
		static std::string GetUUID(const std::string& path);
		static Ref<Material> LoadMaterial(const std::string& uuid);
	private:
		static std::filesystem::path m_CurrentDirectory;

		static std::unordered_map<std::string, std::string> m_UUIDsMap;
	};
}