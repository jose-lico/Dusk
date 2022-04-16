#pragma once
#include "PanelBase.h"

#include "Core/ECS/Entity.h"

#include "Utils/Memory/Memory.h"

#include "glm/glm.hpp"

#include <vector>

namespace DuskEngine
{
	class FrameBuffer;

	class GameViewPortPanel : public Panel
	{
	public:
		GameViewPortPanel(Ref<FrameBuffer>& fb, Entity camera, bool* playing);
		~GameViewPortPanel() = default;

		virtual void OnImGuiRender() override;
	private:
		Ref<FrameBuffer> m_FB;
		glm::vec2 m_ViewportSize;
		Entity m_Camera;
		bool* m_Playing;


		//void EditorCamera();
		//bool m_IsLeftMousePressed = false;
		//bool m_HasFirstMousePosition = false;
		//glm::vec2 m_LastMousePosition;
	};
}