#pragma once
#include "PanelBase.h"

#include "Core/ECS/Entity.h"

#include "Utils/Memory/Memory.h"

#include "glm/glm.hpp"

#include <vector>

namespace DuskEngine
{
	struct Framebuffer;
	class EditorCamera;
	class KeyPressedEvent;
	
	class SceneViewportPanel : public Panel
	{
	public:
		SceneViewportPanel(Framebuffer* fb, EditorCamera* camera);
		~SceneViewportPanel() = default;

		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;

		void SelectedEntities(std::vector<Entity>& entities) { m_SelectedEntities = &entities; }
		bool Test(KeyPressedEvent& e);
	private:
		std::vector<Entity>* m_SelectedEntities = nullptr;

		Framebuffer* m_FB;

		glm::vec2 m_ViewportSize;
		EditorCamera* m_Camera;

		void MoveEditorCamera();
		bool m_IsLeftMousePressed = false;
		bool m_HasFirstMousePosition = false;
		glm::vec2 m_LastMousePosition;
		unsigned int m_ImGuizmoOperation;
	};
}