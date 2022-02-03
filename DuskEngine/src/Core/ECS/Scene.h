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
		
		template<typename T>
		Entity* FindEntityOfType()
		{
			auto view = m_Registry.view<T>();
			return view.size() > 0 ? new Entity(view.front(), this) : nullptr;
		}

		Entity* GetMainCamera();

		void OnUpdate();
	private:
		entt::registry m_Registry;

		friend class Entity;
		friend class HierarchyPanel;
		friend class SceneSerializer;
	};
}