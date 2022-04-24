#pragma once
#include "Core/Macros/DUSK_API.h"

#include "entt/entity/registry.hpp"

namespace DuskEngine
{
	class Entity;
	class EditorCamera;

	class DUSK_EXPORT Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name);
		Entity CreateEmptyEntity();
		
		void DestroyEntity(entt::entity handle);
		void DestroyEntity(Entity entity);
		
		template<typename T>
		Entity* FindEntityOfType()
		{
			auto view = m_Registry.view<T>();
			return view.size() > 0 ? new Entity(view.front(), this) : nullptr;
		}

		Entity* GetMainCamera();
		Entity* FindEntity(const std::string& name);

		void OnUpdateEditor(EditorCamera& camera);
		void OnAwakeRuntime();
		void OnUpdateRuntime(bool running);
	private:
		entt::registry m_Registry;

		bool playing = true;

		friend class Entity;
		friend class HierarchyPanel;
		friend class SceneSerializer;
	};
}