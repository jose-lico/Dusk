#include "pch.h"
#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace DuskEngine {

#ifdef DUSK_OPENGL
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;
#elif DUSK_D3D11
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::D3D11;
#endif

	RendererAPI* RendererAPI::Create()
	{
		switch (s_API)
		{
			case RendererAPI::API::None:    return nullptr;
			case RendererAPI::API::OpenGL:  return new OpenGLRendererAPI();
			case RendererAPI::API::D3D11:	return nullptr;
			default:						return nullptr;
		}
	}
}