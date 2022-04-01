#pragma once
#include "Core/Application/Layer.h"

#include "Utils/Memory/Memory.h"

#include "Panels.h"

#include <vector>

namespace DuskEngine
{
	class Scene;
	class FrameBuffer;

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