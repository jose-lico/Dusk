#include "Core/ECS/Scene.h"
#include "Utils/Memory/Memory.h"

#include <uuid.h>

namespace DuskEngine
{
	class ResourceManager
	{
	public:
		ResourceManager();
		~ResourceManager();

		// Loads all UUIDs in the project
		void LoadUUIDs();

		// Checks all the UUIDs present in the scene, loads the corresponding resources
		void LoadResources(Ref<Scene>& scene);
	private:
	};
}