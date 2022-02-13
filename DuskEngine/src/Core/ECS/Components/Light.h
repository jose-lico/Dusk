#pragma once

#include <glm/glm.hpp>
#include <visit_struct/visit_struct.hpp>

namespace DuskEngine
{
	enum class LightType { Directional, Point, Spot};

	struct Light
	{
		LightType type;
		glm::vec3 color;

		Light() :type(LightType::Directional), color(1.0f) {}

		static std::string LightTypeString(LightType type)
		{
			switch (type)
			{
			case LightType::Directional:
				return "Directional";
			case LightType::Point:
				return "Point";
			case LightType::Spot:
				return "Spot"; 
			default:
				return "Unknown";
			}
		}

		static LightType LightTypeEnum(const std::string& type)
		{
			if (type == "Directional")
				return LightType::Directional;
			else if (type == "Point")
				return LightType::Point;
			else if (type == "Spot")
				return LightType::Spot;
			else if (type == "Unknown")
				return LightType::Directional;
		}
	};
}

VISITABLE_STRUCT(DuskEngine::Light, type, color);