#pragma once
#include "PanelBase.h"

#include "DuskEngine.h"

#include <glm/glm.hpp>
#include <memory>

class ScenePanel : public Panel
{
public:
	ScenePanel(std::shared_ptr<DuskEngine::FrameBuffer>& fb, DuskEngine::Camera& camera);
	~ScenePanel();
	virtual void OnImGuiRender() override;
private:
	glm::vec2 m_ViewportSize;
	std::shared_ptr<DuskEngine::FrameBuffer> m_FB;
	DuskEngine::Camera* m_Camera;
};