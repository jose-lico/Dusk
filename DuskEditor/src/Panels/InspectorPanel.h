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
		void SelectedEntities(std::vector<Entity*>& entities) { m_SelectedEntities = &entities; }
	private:
		std::vector<Entity*>* m_SelectedEntities = nullptr;

		static void CameraInspector(std::vector<Camera*>& cameras);
		static void TransformInspector(std::vector<Transform*>& transforms);
		static void LightInspector(std::vector<Light*>& lights);
		static void MaterialInspector(std::vector<MeshRenderer*>& lights);
	};
}