#include "pch.h"
#include "RendererContext.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace DuskEngine
{
#ifdef DUSK_OPENGL
	RendererContext::API RendererContext::s_API = RendererContext::API::OpenGL;
#elif DUSK_D3D11
	RendererContext::API RendererContext::s_API = RendererContext::API::D3D11;
#endif

	UniqueRef<RendererContext> RendererContext::Create(void* window)
	{
		switch (s_API)
		{
			case RendererContext::API::None:    return nullptr;
			case RendererContext::API::OpenGL:  return MakeUnique<OpenGLContext>((GLFWwindow*)window);
			case RendererContext::API::D3D11:	return nullptr;
			default:							return nullptr;
		}
	}
}