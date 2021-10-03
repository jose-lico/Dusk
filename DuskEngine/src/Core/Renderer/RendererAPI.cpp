#include "pch.h"
#include "RendererAPI.h"

#include "RendererContext.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace DuskEngine {

	RendererAPI* RendererAPI::Create()
	{
		switch (RendererContext::GetAPI())
		{
			case RendererContext::API::None:    return nullptr;
			case RendererContext::API::OpenGL:  return new OpenGLRendererAPI();
			case RendererContext::API::D3D11:	return nullptr;
			default:							return nullptr;
		}
	}
}