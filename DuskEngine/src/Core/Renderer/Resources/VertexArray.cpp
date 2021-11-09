#include "pch.h"
#include "VertexArray.h"

#include "Core/Renderer/RendererContext.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace DuskEngine
{
	VertexArray* VertexArray::Create()
	{
		switch (RendererContext::GetAPI())
		{
			case RendererContext::API::None:    return nullptr;
			case RendererContext::API::OpenGL:  return new OpenGLVertexArray();
			case RendererContext::API::D3D11:	return nullptr;
			default:							return nullptr;
		}
	}
}