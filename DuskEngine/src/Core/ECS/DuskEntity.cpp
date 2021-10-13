#include "pch.h"
#include "DuskEntity.h"

namespace DuskEngine
{
	void DuskEntity::AddComponent(std::shared_ptr<Component> comp)
	{
		m_Components.push_back(comp);
	}

	void DuskEntity::UpdateComponents()
	{
		for (std::shared_ptr<Component> comp : m_Components)
			comp->UpdateComponent();
	}

	void DuskEntity::ImGuiRender()
	{
		for (std::shared_ptr<Component> comp : m_Components)
			comp->ImGuiComponent();
	}
}
