#pragma once
#include "Core/Renderer/Resources/Shader.h"

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

		virtual void SetUniformInt(const std::string& name, int i) override;
		virtual void SetUniformFloat(const std::string& name, float f) override;
		virtual void SetUniformVec2(const std::string& name , const glm::vec2& v) override;
		virtual void SetUniformVec3(const std::string& name , const glm::vec3& v) override;
		virtual void SetUniformVec4(const std::string& name , const glm::vec4& v) override;
		virtual void SetUniformMat3(const std::string& name , const glm::mat3& m) override;
		virtual void SetUniformMat4(const std::string& name , const glm::mat4& m) override;
	private:
		std::unordered_map<std::string, int> m_uniformLocations;
		unsigned int m_ID;
		std::string m_Filepath;

		UniformType GetType(const std::string& type);
		int GetUniformLocation(const std::string& name);
		unsigned int CompileShader(unsigned int type, const std::string& source);
		unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
		ShaderProgramSource ParseShader();
	};
}