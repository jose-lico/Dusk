#include "pch.h"
#include "TransformComponent.h"

#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>

namespace DuskEngine
{
	TransformComponent::TransformComponent()
	{
		modelMatrix = std::make_shared<glm::mat4>(1.0f);
	}

	void TransformComponent::UpdateComponent()
	{
		*modelMatrix = glm::translate(glm::mat4(1.0), position);
		//modelMatrix = glm::rotate(modelMatrix, glm::normalize(rotation));
		//modelMatrix = glm::rotate;
	}

	void TransformComponent::ImGuiComponent()
	{
		ImGui::Begin("Transform");
		ImGui::DragFloat3("Position", &position[0], .01f);
		ImGui::End();
	}
}
