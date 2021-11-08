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

		void EditorCamera();
	private:
		Dockspace m_Dockspace;
		std::vector<Panel*> m_Panels;

		std::shared_ptr<Scene> m_Scene;
		//InspectorPanel* inspector;
		Entity camera;
		std::unique_ptr<InspectorPanel> inspector;

		std::shared_ptr<FrameBuffer> m_FB;

		// camera stuff remove later
		bool movingCamera = false;
		bool firstMouse = false;
		float lastX = WindowManager::GetWindow()->GetWidth() / 2.0f;
		float lastY = WindowManager::GetWindow()->GetWidth() / 2.0f;
	};
}