#pragma once

#include <glm/glm.hpp>
#include <string>

namespace DuskEngine
{
	// All experimental, still not in use

	class ImGuiUtils
	{
	public:
		static void DrawVec3(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);
		static void DrawVec3Table(const std::string& label, glm::vec3& values);
		static void DrawFloatTable(const std::string& label, float& value);
		static void DrawTextTable(const std::string& label);
		static void DrawPropertyName(const std::string& label);
	};
}