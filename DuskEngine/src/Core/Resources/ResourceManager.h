#include "Core/ECS/Scene.h"
#include "Utils/Memory/Memory.h"

#include <uuid.h>

#include <unordered_map>
#include <filesystem>

namespace DuskEngine
{
	class ResourceManager
	{
	public:
		ResourceManager();
		~ResourceManager();

		// Loads all UUIDs in the project
		void CreateUUIDs();
		void LoadUUIDs();

		// Checks all the UUIDs present in the scene, loads the corresponding resources
		void LoadResources(Ref<Scene>& scene);
	private:
		std::filesystem::path m_CurrentDirectory;


		std::unordered_map<std::string, std::string> m_UUIDsMap;
	};
}