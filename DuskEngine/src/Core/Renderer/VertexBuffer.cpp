#include "pch.h"
#include "VertexBuffer.h"

#include "RendererContext.h"

#include "Platform/OpenGL/OpenGLVertexBuffer.h"

namespace DuskEngine
{
	VertexBuffer* VertexBuffer::Create(const void* data, int size)
	{
		switch (RendererContext::GetAPI())
		{
			case RendererContext::API::None:    return nullptr;
			case RendererContext::API::OpenGL:  return new OpenGLVertexBuffer(data, size);
			case RendererContext::API::D3D11:	return nullptr;
			default:							return nullptr;
		}
	}
}