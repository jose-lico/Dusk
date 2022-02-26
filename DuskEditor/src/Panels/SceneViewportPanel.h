#pragma once
#include "PanelBase.h"

#include "DuskEngine.h"

namespace DuskEngine
{
	class SceneViewportPanel : public Panel
	{
	public:
		SceneViewportPanel(Ref<FrameBuffer>& fb, Entity camera);
		~SceneViewportPanel() = default;

		virtual void OnImGuiRender() override;
		void SelectedEntities(std::vector<Entity*>& entities) { m_SelectedEntities = &entities; }
	private:
		void EditorCamera();

		std::vector<Entity*>* m_SelectedEntities = nullptr; // TODO

		Ref<FrameBuffer> m_FB;
		glm::vec2 m_ViewportSize;
		Entity m_Camera;

		bool m_IsLeftMousePressed = false;
		bool m_HasFirstMousePosition = false;
		glm::vec2 m_LastMousePosition;
	};
}