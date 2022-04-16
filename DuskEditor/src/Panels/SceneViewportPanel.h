#pragma once
#include "PanelBase.h"

#include "Core/ECS/Entity.h"

#include "Utils/Memory/Memory.h"

#include "glm/glm.hpp"

#include <vector>

namespace DuskEngine
{
	class FrameBuffer;
	
	class SceneViewportPanel : public Panel
	{
	public:
		SceneViewportPanel(Ref<FrameBuffer>& fb, Entity camera);
		~SceneViewportPanel() = default;

		virtual void OnImGuiRender() override;
		void SelectedEntities(std::vector<Entity>& entities) { m_SelectedEntities = &entities; }
	private:
		std::vector<Entity>* m_SelectedEntities = nullptr;

		Ref<FrameBuffer> m_FB;
		glm::vec2 m_ViewportSize;
		Entity m_Camera;

		void EditorCamera();
		bool m_IsLeftMousePressed = false;
		bool m_HasFirstMousePosition = false;
		glm::vec2 m_LastMousePosition;
	};
}