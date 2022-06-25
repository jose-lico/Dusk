#include "pch.h"
#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace DuskEngine
{
	Ref<Shader> Shader::Create(const std::string& filepath, const std::string& name)
	{
		return MakeRef<OpenGLShader>(filepath, name);
	}

	Ref<Shader> Shader::Create(const std::filesystem::path& path, const uuids::uuid& uuid)
	{
		return MakeRef<OpenGLShader>(path, uuid);
	}
}