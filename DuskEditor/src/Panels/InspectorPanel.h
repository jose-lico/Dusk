#pragma once
#include "PanelBase.h"

#include "Core/ECS/Components.h"

namespace DuskEngine
{
	class Entity;

	class InspectorPanel : public Panel
	{
	public:
		InspectorPanel();
		~InspectorPanel() = default;

		virtual void OnImGuiRender() override;
		void SelectedEntities(std::vector<Entity>& entities) { m_SelectedEntities = &entities; }
	private:
		std::vector<Entity>* m_SelectedEntities = nullptr;

		static void MetaInspector(std::vector<Meta*>& metas);
		static void TransformInspector(std::vector<Transform*>& transforms);
		static void CameraInspector(std::vector<Camera*>& cameras);
		static void LightInspector(std::vector<Light*>& lights);
		static void MaterialInspector(std::vector<MeshRenderer*>& materials);
		static void ScriptInspector(std::vector<Script*>& scripts);
	};
}