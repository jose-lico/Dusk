#include "pch.h"
#include "OpenGLShader.h"
#include "GLCommon.h"

#include "Core/Macros/LOG.h"

namespace DuskEngine
{
	// TODO -> Rework or remove
	OpenGLShader::OpenGLShader(const std::string& filepath, const std::string& name)
	{
		if (name.empty())
		{
			std::string::size_type begin = filepath.rfind("/") + 1;
			std::string::size_type end = filepath.find(".");
			std::string filename = filepath.substr(begin, end - begin);
			m_Name = filename;
		}
		else
			m_Name = name;

		m_Path = filepath;

		ShaderProgramSource source = ParseShader();

		m_ID = CreateShader(source.VertexSource, source.FragmentSource);
		
		LOG("Created Shader " + m_Name)
	}

	OpenGLShader::OpenGLShader(const std::filesystem::path& path, const uuids::uuid& uuid)
	{
		m_UUID = uuid;
		m_Path = path;
		m_Name = path.filename().string();
		
		ShaderProgramSource source = ParseShader();
		
		m_ID = CreateShader(source.VertexSource, source.FragmentSource);
		
		LOG("Created Shader " + m_Name)
	}

	OpenGLShader::~OpenGLShader()
	{
		LOG("Deleted Shader " + m_Name)
		glDeleteProgram(m_ID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_ID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	unsigned int OpenGLShader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
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

	unsigned int OpenGLShader::CompileShader(unsigned int type, const std::string& source)
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
			ERROR("ERROR::SHADER::COMPILATION_FAILED") 
			ERROR(infoLog);
			glDeleteShader(id);
			return 0;
		}

		return id;
	}

	ShaderProgramSource OpenGLShader::ParseShader()
	{
		std::ifstream stream(m_Path);

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
				if (line.find("uniform") != std::string::npos && line.find("u_") != std::string::npos) // musnt be an expected uniform
				{
					std::string::size_type typeBegin = line.find(" ") + 1;
					std::string::size_type typeEnd = line.find(" ", typeBegin);
					std::string type = line.substr(typeBegin, typeEnd - typeBegin);

					std::string::size_type nameBegin = line.find("u_") + 2;
					std::string::size_type nameEnd = line.find(";", nameBegin);
					std::string name = line.substr(nameBegin, nameEnd - nameBegin);

					UniformType uniformType;
					uniformType = GetType(type);
					UniformSpecs.push_back(UniformSpec(name, uniformType));
				}
				ss[(int)type] << line << '\n';
			}
		}

		return { ss[0].str(), ss[1].str() };
	}
	
	UniformType OpenGLShader::GetType(const std::string& type)
	{
		if (type == "vec3")
			return UniformType::Vec3;
		if (type == "sampler2D")
			return UniformType::Texture;

		return UniformType::Unknown;
	}

	int OpenGLShader::GetUniformLocation(const std::string& name)
	{
		if (m_uniformLocations.find(name) != m_uniformLocations.end())
			return m_uniformLocations[name];

		int location = glGetUniformLocation(m_ID, name.c_str());

		m_uniformLocations[name] = location;

		return location;
	}

	void OpenGLShader::SetUniformInt(const std::string& name, int i)
	{
		glUniform1i(GetUniformLocation(name), i);
	}

	void OpenGLShader::SetUniformFloat(const std::string& name, float f)
	{
		glUniform1f(GetUniformLocation(name), f);
	}

	void OpenGLShader::SetUniformVec2(const std::string& name, const glm::vec2& v)
	{
		glUniform2f(GetUniformLocation(name), v.x, v.y);
	}

	void OpenGLShader::SetUniformVec3(const std::string& name, const glm::vec3& v)
	{
		glUniform3f(GetUniformLocation(name), v.x, v.y, v.z);
	}

	void OpenGLShader::SetUniformVec4(const std::string& name, const glm::vec4& v)
	{
		glUniform4f(GetUniformLocation(name), v.x, v.y, v.z, v.w);
	}

	void OpenGLShader::SetUniformMat3(const std::string& name, const glm::mat3& m)
	{
		glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &m[0][0]);
	}

	void OpenGLShader::SetUniformMat4(const std::string& name, const glm::mat4& m)
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &m[0][0]);
	}
}