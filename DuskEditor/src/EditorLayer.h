#pragma once
#include "DuskEngine.h"

#include "Panels.h"

class EditorLayer : public DuskEngine::Layer
{
public:
	EditorLayer();
	~EditorLayer() {};
	void OnAttach() override;
	void OnUpdate() override;
	void OnImGuiRender() override;
private:
	Dockspace m_Dockspace;
	std::vector<PanelBase*> m_Panels;

	std::shared_ptr<DuskEngine::FrameBuffer> m_FB;
	std::shared_ptr<DuskEngine::VertexArray> m_VA;
	std::shared_ptr<DuskEngine::Shader> m_Shader;
	std::shared_ptr<DuskEngine::Texture> m_Texture;
	std::shared_ptr<DuskEngine::Camera> m_Camera;

	bool movingCamera = false;
	bool firstMouse = false;
	float lastX = DuskEngine::WindowManager::GetWindow()->GetWidth() / 2.0f;
	float lastY = DuskEngine::WindowManager::GetWindow()->GetWidth() / 2.0f;

	std::shared_ptr<DuskEngine::SceneEntt> m_SceneEntt;

	InspectorPanel* inspector;
};