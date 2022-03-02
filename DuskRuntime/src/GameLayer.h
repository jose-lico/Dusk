#pragma once
#include "DuskEngine.h"

namespace DuskEngine
{
	class GameLayer : public Layer
	{
	public:
		GameLayer();
		~GameLayer() = default;
		void OnUpdate() override;
	private:
		void Camera();

		Entity m_Camera;

		bool m_IsLeftMousePressed = false;
		bool m_HasFirstMousePosition = false;
		glm::vec2 m_LastMousePosition;

		Ref<Scene> m_Scene;
	};
}