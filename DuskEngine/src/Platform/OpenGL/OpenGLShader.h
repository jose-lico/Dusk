#pragma once
#include "Core/Renderer/Shader.h"

#include <unordered_map>

namespace DuskEngine 
{
	// temp
	struct ShaderProgramSource
	{
		std::string VertexSource;
		std::string FragmentSource;
	};

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		~OpenGLShader();

		void Bind() const override;
		void Unbind() const override;

		void SetUniformFloat(const std::string& name, float f) override;
		void SetUniformVec2(const std::string& name , const glm::vec2& v) override;
		void SetUniformVec3(const std::string& name , const glm::vec3& v) override;
		void SetUniformVec4(const std::string& name , const glm::vec4& v) override;
		void SetUniformMat3(const std::string& name , const glm::mat3& m) override;
		void SetUniformMat4(const std::string& name , const glm::mat4& m) override;
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