#pragma once
#include "PanelBase.h"

#include "DuskEngine.h"

namespace DuskEngine
{
	class SceneViewportPanel : public PanelBase
	{
	public:
		SceneViewportPanel(std::shared_ptr<FrameBuffer>& fb, std::shared_ptr<Camera> camera);
		~SceneViewportPanel() = default;

		virtual void OnImGuiRender() override;
	private:
		std::shared_ptr<FrameBuffer> m_FB;
		std::shared_ptr<Camera> m_Camera;
		glm::vec2 m_ViewportSize;
	};
}