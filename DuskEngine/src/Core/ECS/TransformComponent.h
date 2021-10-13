#pragma once
#include "Core/Macros/DUSK_API.h"

#include "ComponentBase.h"

#include <glm/glm.hpp>

#include <memory>

namespace DuskEngine
{
	class DUSK_API TransformComponent : public Component
	{
	public:
		TransformComponent();

		glm::vec3 position;
		glm::vec3 scale;
		glm::vec3 rotation;
		std::shared_ptr<glm::mat4> modelMatrix;
		virtual void UpdateComponent() override;
		virtual void ImGuiComponent() override;
	};
}