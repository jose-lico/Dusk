#pragma once
#include "Core/Macros/DUSK_API.h"

#include "Scene.h"

#include <entt/entt.hpp>

namespace DuskEngine
{
	class DUSK_EXPORT Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		~Entity();

		template<typename T, typename... Args >
		T& AddComponent(Args&&... args)
		{
			return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
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
	};
}
