#pragma once
#include "Core/Macros/DUSK_API.h"

#include "DuskEntity.h"

#include <vector>

#include <memory>

namespace DuskEngine
{
	class DUSK_API Scene
	{
	public:
		Scene();
		~Scene();

		void AddEntity(std::shared_ptr<DuskEntity> entity);

		void OnEntityUpdate();
		void OnEntityImGui();
	private:
		std::vector<std::shared_ptr<DuskEntity>> m_Entities;
	};
}