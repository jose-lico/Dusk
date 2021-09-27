#include "pch.h"
#include "IndexBuffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLIndexBuffer.h"

namespace DuskEngine
{
	IndexBuffer* IndexBuffer::Create(const void* data, int count)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    return nullptr;
			case RendererAPI::API::OpenGL:  return new OpenGLIndexBuffer(data, count);
		}

		return nullptr;
		
	}
}