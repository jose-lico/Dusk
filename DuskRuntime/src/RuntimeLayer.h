#pragma once
#include "Core/Application/Layer.h"

#include "Core/ECS/Entity.h"
#include "Core/Events/WindowEvent.h"

#include "Utils/Memory/Memory.h"

#include "glm/glm.hpp"

namespace DuskEngine
{


	class RuntimeLayer : public Layer
	{
	public:
		RuntimeLayer();
		~RuntimeLayer();
		void OnUpdate() override;
		void OnEvent(Event& event) override;
	private:
		bool CameraProj(WindowResizeEvent& event);
		/*void Camera();

		Entity m_Camera;

		bool m_IsLeftMousePressed = false;
		bool m_HasFirstMousePosition = false;
		glm::vec2 m_LastMousePosition;*/

		Ref<Scene> m_Scene;
	};
}