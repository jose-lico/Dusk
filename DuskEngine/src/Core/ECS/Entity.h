#pragma once
#include "Core/Application/Core.h"

#include "Scene.h"

#include "entt/entt.hpp"

namespace DuskEngine
{
	class Entity
	{
		struct Meta;
		struct Transform;

	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		~Entity();

		// Can only have one of each component. In the future only certain components will have this restriction, such as Transform or Meta.
		template<typename T, typename... Args >
		T& AddComponent(Args&&... args)
		{
			if (!HasComponent<T>())
				return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			else
			{
				WARN("Component already present!");
				return GetComponent<T>();
			}
		}

		template<typename T>
		void RemoveComponent()
		{
			#ifdef DUSK_WINDOWS
			if (!std::is_same<T, DuskEngine::Meta>() && !std::is_same<T, DuskEngine::Transform>())
				m_Scene->m_Registry.remove<T>(m_EntityHandle);
			else
				WARN("Can not remove Meta or Transform component!");
			#elif DUSK_LINUX
			if (!std::is_same<T, Meta>() && !std::is_same<T, Transform>())
				m_Scene->m_Registry.remove<T>(m_EntityHandle);
			else
				WARN("Can not remove Meta or Transform component!");
			#endif
		}

		template<typename T>
		T& GetComponent()
		{
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.any_of<T>(m_EntityHandle);
		}
	private:
		entt::entity m_EntityHandle;
		Scene* m_Scene;

		friend class Scene;
		friend class HierarchyPanel;
	};
}
