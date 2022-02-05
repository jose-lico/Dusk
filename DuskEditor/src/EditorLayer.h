#pragma once
#include "DuskEngine.h"

#include "Panels.h"

namespace DuskEngine
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		~EditorLayer();
		void OnAttach() override;
		void OnUpdate() override;
		void OnImGuiRender() override;
	private:
		Dockspace m_Dockspace;
		std::vector<Panel*> m_Panels;

		Ref<Scene> m_Scene;
		Ref<FrameBuffer> m_FB;
	};
}