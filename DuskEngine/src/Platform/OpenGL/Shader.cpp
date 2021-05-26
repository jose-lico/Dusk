#include "pch.h"

#include "Shader.h"
#include "GLCommon.h"

namespace DuskEngine
{
	Shader::Shader(const std::string& filepath)
		:m_Filepath(filepath), m_ID(0)
	{
		ShaderProgramSource source = ParseShader();
		m_ID = CreateShader(source.VertexSource, source.FragmentSource);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_ID);
	}

	void Shader::Bind() const
	{
		glUseProgram(m_ID);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
	{
		unsigned int program = glCreateProgram();
		unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
		unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

		glAttachShader(program, vs);
		glAttachShader(program, fs);
		glLinkProgram(program);
		glValidateProgram(program);

		glDeleteShader(vs);
		glDeleteShader(fs);

		return program;
	}

	unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
	{
		unsigned int id = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		int success;
		char infoLog[512];
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(id, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
			glDeleteShader(id);
			return 0;
		}

		return id;
	}

	ShaderProgramSource Shader::ParseShader()
	{
		std::ifstream stream(m_Filepath);

		enum class ShaderType
		{
			NONE = -1, VERTEX = 0, FRAGMENT = 1
		};

		std::string line;
		std::stringstream ss[2];
		ShaderType type = ShaderType::NONE;
		while (getline(stream, line))
		{
			if (line.find("#shader") != std::string::npos)
			{
				if (line.find("vertex") != std::string::npos)
				{
					type = ShaderType::VERTEX;
				}
				else if (line.find("fragment") != std::string::npos)
				{
					type = ShaderType::FRAGMENT;
				}
			}
			else
			{
				ss[(int)type] << line << '\n';
			}
		}

		return { ss[0].str(), ss[1].str() };
	}
	
	int Shader::GetUniformLocation(const std::string& name)
	{
		if (m_uniformLocations.find(name) != m_uniformLocations.end())
			return m_uniformLocations[name];

		int location = glGetUniformLocation(m_ID, name.c_str());
		//if (location == -1)
			//std::cout << "Warning: uniform " << name << " does not exist." << std::endl;

		m_uniformLocations[name] = location;

		return location;
	}

	void Shader::SetUniformFloat(const std::string& name, float f)
	{
		glUniform1f(GetUniformLocation(name), f);
	}

	void Shader::SetUniformVec2(const std::string& name, glm::vec2& v)
	{
		glUniform2f(GetUniformLocation(name), v.x, v.y);
	}

	void Shader::SetUniformVec3(const std::string& name, glm::vec3& v)
	{
		glUniform3f(GetUniformLocation(name), v.x, v.y, v.z);
	}

	void Shader::SetUniformVec4(const std::string& name, glm::vec4& v)
	{
		glUniform4f(GetUniformLocation(name), v.x, v.y, v.z, v.w);
	}

	void Shader::SetUniformMat3(const std::string& name, glm::mat3& m)
	{
		glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &m[0][0]);
	}

	void Shader::SetUniformMat4(const std::string& name, glm::mat4& m)
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &m[0][0]);
	}
}