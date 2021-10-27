#pragma once
#include "Core/Macros/DUSK_API.h"

#include "Core/Renderer/Camera.h"

#include <entt/entity/registry.hpp>

namespace DuskEngine
{
	class Entity;

	class DUSK_API Scene
	{
	public:
		Scene(std::shared_ptr<Camera>& camera);
		~Scene();
		Entity CreateEntity(const std::string& name);
		void OnUpdate();
	private:
		entt::registry m_Registry;

		std::shared_ptr<Camera> m_Camera;

		friend class Entity;
		friend class HierarchyPanel;
	};
}