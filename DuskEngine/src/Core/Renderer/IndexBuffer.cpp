#include "pch.h"
#include "IndexBuffer.h"

#include "RendererContext.h"

#include "Platform/OpenGL/OpenGLIndexBuffer.h"

namespace DuskEngine
{
	IndexBuffer* IndexBuffer::Create(const void* data, int count)
	{
		switch (RendererContext::GetAPI())
		{
			case RendererContext::API::None:    return nullptr;
			case RendererContext::API::OpenGL:  return new OpenGLIndexBuffer(data, count);
			case RendererContext::API::D3D11:	return nullptr;
			default:							return nullptr;
		}		
	}
}