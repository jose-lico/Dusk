#pragma once

#include "pch.h"

#include "glm.hpp"

namespace DuskEngine
{
	class Shader
	{
	public:
		Shader(const std::string& filepath);
		~Shader();

		void Bind();
		void Unbind();

		void SetUniformFloat(const std::string& name, float f);
		void SetUniformVec2(const std::string& name, glm::vec2& v);
		void SetUniformVec3(const std::string& name, glm::vec3& v);
		void SetUniformVec4(const std::string& name, glm::vec4& v);
		void SetUniformMat3(const std::string& name, glm::mat3& m);
		void SetUniformMat4(const std::string& name, glm::mat4& m);
	private:
		unsigned int m_ID;
	};
}