#pragma once
#include "DuskEngine.h"

#include "Panels.h"

namespace DuskEngine
{
	class EditorLayer : public Layer
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

		std::shared_ptr<FrameBuffer> m_FB;
		std::shared_ptr<VertexArray> m_VA;
		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<Texture> m_Texture;
		std::shared_ptr<Camera> m_Camera;

		bool movingCamera = false;
		bool firstMouse = false;
		float lastX = WindowManager::GetWindow()->GetWidth() / 2.0f;
		float lastY = WindowManager::GetWindow()->GetWidth() / 2.0f;

		std::shared_ptr<SceneEntt> m_SceneEntt;

		InspectorPanel* inspector;
	};
}