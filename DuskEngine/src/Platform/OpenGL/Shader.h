#pragma once
#include "pch.h"

#include "Core/Application/Macros.h"
#include "glm.hpp"

namespace DuskEngine 
{
	// temp
	struct ShaderProgramSource
	{
		std::string VertexSource;
		std::string FragmentSource;
	};

	class DUSK_API Shader
	{
	public:
		Shader(const std::string& filepath);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void SetUniformFloat(const std::string& name, float f);
		void SetUniformVec2(const std::string& name , const glm::vec2& v);
		void SetUniformVec3(const std::string& name , const glm::vec3& v);
		void SetUniformVec4(const std::string& name , const glm::vec4& v);
		void SetUniformMat3(const std::string& name , const glm::mat3& m);
		void SetUniformMat4(const std::string& name , const glm::mat4& m);
	private:
		std::unordered_map<std::string, int> m_uniformLocations;
		unsigned int m_ID;
		std::string m_Filepath;
		int GetUniformLocation(const std::string& name);
		unsigned int CompileShader(unsigned int type, const std::string& source);
		unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
		ShaderProgramSource ParseShader();
	};
}