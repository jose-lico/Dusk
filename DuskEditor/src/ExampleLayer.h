#pragma once
#include "DuskEngine.h"

class ExampleLayer : public DuskEngine::Layer
{
public:
	ExampleLayer();
	~ExampleLayer() {};
	void OnAttach() override;
	void OnUpdate() override;
private:
	std::shared_ptr<DuskEngine::VertexArray> m_VA;
	std::shared_ptr<DuskEngine::Shader> m_Shader;
	std::shared_ptr<DuskEngine::Texture> m_Texture;
	DuskEngine::Camera* m_Camera;

	bool movingCamera = false;
	bool firstMouse = false;
	float lastX = DuskEngine::WindowManager::GetWindow()->GetWidth() / 2.0f;
	float lastY = DuskEngine::WindowManager::GetWindow()->GetWidth() / 2.0f;
};