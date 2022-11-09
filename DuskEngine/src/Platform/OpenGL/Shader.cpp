#include "pch.h"
#include "Shader.h"

#include "OpenGLAPI.h"

namespace DuskEngine
{
	ShaderProgramSource ParseShader(Shader& shader);
	UniformType GetType(const std::string& type);
	void CreateProgram(Shader& shader, const std::string& vertexShader, const std::string& fragmentShader);

	Shader CreateShader(const std::filesystem::path& path, const uuids::uuid& uuid)
	{
		Shader shader;
		shader.UUID = uuid;
		shader.Path = path;
		shader.Name = path.filename().string();

		ShaderProgramSource source = ParseShader(shader);

		CreateProgram(shader, source.VertexSource, source.FragmentSource);

		LOG("Created Shader " + shader.Name);

		return shader;
	}

	ShaderProgramSource ParseShader(Shader& shader)
	{
		std::ifstream stream(shader.Path);

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
					shader.UniformSpecs.push_back(UniformSpec(name, uniformType));
				}
				ss[(int)type] << line << '\n';
			}
		}

		return { ss[0].str(), ss[1].str() };
	}

	UniformType GetType(const std::string& type)
	{
		if (type == "vec3")
			return UniformType::Vec3;
		if (type == "sampler2D")
			return UniformType::Texture;

		return UniformType::Unknown;
	}

	void CreateProgram(Shader& shader, const std::string& vertexShader, const std::string& fragmentShader)
	{
		OpenGLAPI::CreateProgram(shader, vertexShader, fragmentShader);
	}
}