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
	private:
		void EditorCamera();

		Ref<FrameBuffer> m_FB;
		glm::vec2 m_ViewportSize;
		Entity m_Camera;

		bool m_IsLeftMousePressed = false;
		bool m_HasFirstMousePosition = false;
		glm::vec2 m_LastMousePosition;
	};
}