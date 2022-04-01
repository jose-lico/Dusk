#pragma once
#include "Core/Application/Layer.h"

#include "Core/ECS/Entity.h"

#include "Utils/Memory/Memory.h"

#include "glm/glm.hpp"

namespace DuskEngine
{


	class RuntimeLayer : public Layer
	{
	public:
		RuntimeLayer();
		~RuntimeLayer() = default;
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