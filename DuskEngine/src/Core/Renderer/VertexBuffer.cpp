#include "pch.h"
#include "VertexBuffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexBuffer.h"

namespace DuskEngine
{
	VertexBuffer* VertexBuffer::Create(const void* data, int size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    return nullptr;
			case RendererAPI::API::OpenGL:  return new OpenGLVertexBuffer(data, size);
		}

		return nullptr;
	}
}