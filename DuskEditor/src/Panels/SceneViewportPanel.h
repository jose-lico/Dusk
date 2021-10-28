#pragma once
#include "PanelBase.h"

#include "DuskEngine.h"

namespace DuskEngine
{
	class SceneViewportPanel : public PanelBase
	{
	public:
		SceneViewportPanel(std::shared_ptr<FrameBuffer>& fb, Entity camera);
		~SceneViewportPanel() = default;

		virtual void OnImGuiRender() override;
	private:
		std::shared_ptr<FrameBuffer> m_FB;
		glm::vec2 m_ViewportSize;
		Entity m_Camera;
	};
}