#include "pch.h"
#include "Texture.h"

#include "../RendererContext.h"

#include "Platform/OpenGL/OpenGLTexture.h"

namespace DuskEngine
{
    Texture* Texture::Create(const char* filepath)
    {
		switch (RendererContext::GetAPI())
		{
			case RendererContext::API::None:    return nullptr;
			case RendererContext::API::OpenGL:  return new OpenGLTexture(filepath);
			case RendererContext::API::D3D11:	return nullptr;
			default:							return nullptr;
		}
    }
}