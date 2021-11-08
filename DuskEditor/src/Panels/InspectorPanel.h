#pragma once
#include "PanelBase.h"

#include "DuskEngine.h"

namespace DuskEngine
{
	class InspectorPanel : public Panel
	{
	public:
		InspectorPanel();
		~InspectorPanel() = default;

		virtual void OnImGuiRender() override;
		void SetEntity(DuskEngine::Entity* entity) { m_Entity = entity; }
	private:
		DuskEngine::Entity* m_Entity = nullptr;
	};
}