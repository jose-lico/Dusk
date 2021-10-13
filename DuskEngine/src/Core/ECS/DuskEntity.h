#pragma once
#include "Core/Macros/DUSK_API.h"

#include "ComponentBase.h"

#include <vector>

#include <memory>

namespace DuskEngine
{
	class DUSK_API DuskEntity
	{
	public:
		DuskEntity() = default;
		~DuskEntity() = default;

		void AddComponent(std::shared_ptr<Component> comp);
		void UpdateComponents();
		void ImGuiRender();
	private:
		std::vector<std::shared_ptr<Component>> m_Components;
	};
}