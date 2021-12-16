#include "Core/ECS/Scene.h"

#include "Utils/Memory/Memory.h"

namespace DuskEngine
{
	class SceneSerializer
	{
	public:
		static void SerializeText(const Ref<Scene>& scene, const std::string& path);
		static void SerializeBinary(const Ref<Scene>& scene, const std::string& path);

		static bool DeserializeText(const Ref<Scene>& scene, const std::string& path);
		static bool DeserializeBinary(const Ref<Scene>& scene, const std::string& path);
	};
}