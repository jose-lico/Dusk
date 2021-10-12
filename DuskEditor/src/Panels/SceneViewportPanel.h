#pragma once
#include "PanelBase.h"

#include "DuskEngine.h"

class SceneViewportPanel : public PanelBase
{
public:
	SceneViewportPanel(std::shared_ptr<DuskEngine::FrameBuffer>& fb, std::shared_ptr<DuskEngine::Camera> camera);
	~SceneViewportPanel() = default;

	virtual void OnImGuiRender() override;
private:
	std::shared_ptr<DuskEngine::FrameBuffer> m_FB;
	std::shared_ptr<DuskEngine::Camera> m_Camera;
	glm::vec2 m_ViewportSize;
};