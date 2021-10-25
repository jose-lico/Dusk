#pragma once
#include "Core/Macros/DUSK_API.h"

#include "Core/Renderer/Camera.h"

#include <entt/entity/registry.hpp>

namespace DuskEngine
{
	class Entity;

	class DUSK_API SceneEntt
	{
	public:
		SceneEntt(std::shared_ptr<Camera>& camera);
		~SceneEntt();
		Entity CreateEntity();
		void OnUpdate();
	private:
		entt::registry m_Registry;

		std::shared_ptr<Camera> m_Camera;

		friend class Entity;
		friend class HierarchyPanel;
	};
}