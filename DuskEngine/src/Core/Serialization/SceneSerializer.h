#pragma once

#include "Utils/Memory/Memory.h"

namespace DuskEngine
{
	class Scene;
	class AssetHandler;

	class SceneSerializer
	{
	public:
		static void SerializeText(const Ref<Scene>& scene, const std::filesystem::path& path);
		static void SerializeBinary(const Ref<Scene>& scene, const std::string& path);

		static bool DeserializeText(const Ref<Scene>& scene, const std::filesystem::path& path);
		static bool DeserializeBinary(const Ref<Scene>& scene, const std::string& path);
		static AssetHandler& GetCurrentHandler() { return *m_AssetHandler; }
	private:
		static AssetHandler* m_AssetHandler;
	};
}
