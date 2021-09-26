#include "pch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace DuskEngine
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::None:    return nullptr;
			case RendererAPI::OpenGL:  return new OpenGLVertexArray();
		}

		return nullptr;
	}
}