#pragma once

#include <glm/glm.hpp>
#include <string>

namespace DuskEngine
{
	class ImGuiUtils
	{
	public:
		static void DrawVec3(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);
	};
}