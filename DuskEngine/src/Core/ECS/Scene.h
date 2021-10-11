#pragma once
#include "Core/Macros/DUSK_API.h"

#include <entt/entt.hpp>

namespace DuskEngine
{
	class DuskEntity;

	class DUSK_API Scene
	{
	public:
		Scene();
		~Scene()
		{
			//m_Registry.clear();
		};

		void OnUpdate();
		DuskEntity CreateEntity();
	private:
		entt::registry m_Registry;
		friend class DuskEntity;
	};
}