#pragma once
#include "Core/Macros/DUSK_API.h"

#include <entt/entity/registry.hpp>
#include "glm/glm.hpp"

namespace DuskEngine
{
	struct Transform
	{
		glm::mat4 transform;

		Transform() = default;
		Transform(glm::mat4& t):transform(t){}
	};

	class DUSK_API SceneEntt
	{
	public:
		SceneEntt();
		~SceneEntt();
		entt::entity CreateEntity();

		template<typename T>
		void AddComponent(entt::entity ent)
		{
			m_Registry.emplace<T>(ent);
		}
	private:
		entt::registry m_Registry;
	};
}