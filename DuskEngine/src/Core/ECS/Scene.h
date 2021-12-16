#pragma once
#include "Core/Macros/DUSK_API.h"

#include <entt/entity/registry.hpp>

namespace DuskEngine
{
	class Entity;

	class DUSK_EXPORT Scene
	{
	public:
		Scene();
		~Scene();
		Entity CreateEntity(const std::string& name);
		void OnUpdate();
	private:
		entt::registry m_Registry;

		friend class Entity;
		friend class HierarchyPanel;
		friend class SceneSerializer;
	};
}