#include "pch.h"
#include "Shader.h"

#include "Core/Renderer/RendererContext.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace DuskEngine
{
	Ref<Shader> Shader::Create(const std::string& filepath, const std::string& name)
	{
		switch (RendererContext::GetAPI())
		{
			case RendererContext::API::None:    return nullptr;
			case RendererContext::API::OpenGL:  return MakeRef<OpenGLShader>(filepath, name);
			case RendererContext::API::D3D11:	return nullptr;
			default:							return nullptr;
		}
	}

	Ref<Shader> Shader::Create(const std::filesystem::path& path, const uuids::uuid& uuid)
	{
		switch (RendererContext::GetAPI())
		{
			case RendererContext::API::None:    return nullptr;
			case RendererContext::API::OpenGL:  return MakeRef<OpenGLShader>(path, uuid);
			case RendererContext::API::D3D11:	return nullptr;
			default:							return nullptr;
		}
	}
}