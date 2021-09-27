#include "pch.h"
#include "RendererContext.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace DuskEngine
{
	RendererContext* RendererContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    return nullptr;
			case RendererAPI::API::OpenGL:  return new OpenGLContext((GLFWwindow*)window);
			case RendererAPI::API::D3D11:	return nullptr;
			default:						return nullptr;
		}
	}
}