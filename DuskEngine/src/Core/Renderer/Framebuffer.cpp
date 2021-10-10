#include "pch.h"
#include "Framebuffer.h"

#include "RendererContext.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace DuskEngine
{
	FrameBuffer* FrameBuffer::Create(const FramebufferSpecification& spec)
	{
		switch (RendererContext::GetAPI())
		{
			case RendererContext::API::None:    return nullptr;
			case RendererContext::API::OpenGL:  return new OpenGLFramebuffer(spec);
			case RendererContext::API::D3D11:	return nullptr;
			default:							return nullptr;
		}
	}
}