#pragma once
#include "Core/Macros/DUSK_API.h"

#include "Scene.h"

#include <entt/entt.hpp>

namespace DuskEngine
{
	class DUSK_API DuskEntity
	{
	public:
		DuskEntity() = default;
		DuskEntity(entt::entity handle, Scene* scene);
		~DuskEntity() = default;

		template<typename T, typename... Args>
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
			//return m_Scene->m_Registry.has<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		operator bool() const { return (unsigned int)m_EntityHandle != 0; }

		void DestroyEntity();
	private:
		entt::entity m_EntityHandle { entt::null };
		Scene* m_Scene = nullptr;
	};
}