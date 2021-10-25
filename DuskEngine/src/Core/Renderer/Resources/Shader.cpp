#include "pch.h"
#include "Shader.h"

#include "Core/Renderer/RendererContext.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace DuskEngine
{
	Shader* Shader::Create(const std::string& filepath)
	{
		switch (RendererContext::GetAPI())
		{
			case RendererContext::API::None:    return nullptr;
			case RendererContext::API::OpenGL:  return new OpenGLShader(filepath);
			case RendererContext::API::D3D11:	return nullptr;
			default:							return nullptr;
		}
	}
}