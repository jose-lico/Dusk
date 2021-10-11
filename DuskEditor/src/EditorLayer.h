#pragma once
#include "DuskEngine.h"

#include "Panels/ScenePanel.h"

class EditorLayer : public DuskEngine::Layer
{
public:
	EditorLayer();
	~EditorLayer() {};
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;
	void OnImGuiRender() override;
private:
	std::shared_ptr<DuskEngine::VertexArray> m_VA;
	std::shared_ptr<DuskEngine::Shader> m_Shader;
	std::shared_ptr<DuskEngine::Texture> m_Texture;
	std::shared_ptr<DuskEngine::FrameBuffer> m_FB;
	DuskEngine::Camera* m_Camera;

	bool movingCamera = false;
	bool firstMouse = false;
	float lastX = DuskEngine::WindowManager::GetWindow()->GetWidth() / 2.0f;
	float lastY = DuskEngine::WindowManager::GetWindow()->GetWidth() / 2.0f;

	ScenePanel* scenePanel;
	DuskEngine::DuskEntity m_Entity;

	std::shared_ptr<DuskEngine::Scene> m_Scene;
};