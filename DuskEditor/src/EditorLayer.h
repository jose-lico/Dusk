#pragma once
#include "DuskEngine.h"

#include "glm/glm.hpp"
#include <spdlog/spdlog.h>

class EditorLayer : public DuskEngine::Layer
{
public:
	EditorLayer();
	~EditorLayer() {};
	void OnAttach() override;
	void OnUpdate() override;
	void OnImGuiRender() override;
private:
	std::shared_ptr<DuskEngine::VertexArray> m_VA;
	std::shared_ptr<DuskEngine::Shader> m_Shader;
	std::shared_ptr<DuskEngine::Texture> m_Texture;
	std::shared_ptr<DuskEngine::FrameBuffer> m_FB;
	DuskEngine::Camera* m_Camera;

	std::shared_ptr<spdlog::logger> logger;

	bool movingCamera = false;
	bool firstMouse = false;
	float lastX = DuskEngine::WindowManager::GetWindow()->GetWidth() / 2.0f;
	float lastY = DuskEngine::WindowManager::GetWindow()->GetWidth() / 2.0f;
	glm::vec2 m_ViewportSize;
};