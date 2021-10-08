#pragma once

#include <string>
#include "glm/glm.hpp"
#include "Core/Application/Macros.h"

namespace DuskEngine 
{
	class DUSK_API Shader 
	{
	public:
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetUniformFloat(const std::string& name, float f) = 0;
		virtual void SetUniformVec2(const std::string& name, const glm::vec2& v) = 0;
		virtual void SetUniformVec3(const std::string& name, const glm::vec3& v) = 0;
		virtual void SetUniformVec4(const std::string& name, const glm::vec4& v) = 0;
		virtual void SetUniformMat3(const std::string& name, const glm::mat3& m) = 0;
		virtual void SetUniformMat4(const std::string& name, const glm::mat4& m) = 0;

		static Shader* Create(const std::string& filepath);
	};
}