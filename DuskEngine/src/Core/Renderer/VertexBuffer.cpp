#include "pch.h"
#include "VertexBuffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexBuffer.h"

namespace DuskEngine
{
	VertexBuffer* VertexBuffer::Create(float* vertices, int size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::None:    return nullptr;
			case RendererAPI::OpenGL:  return new OpenGLVertexBuffer(vertices, size);
		}

		return nullptr;
	}
}