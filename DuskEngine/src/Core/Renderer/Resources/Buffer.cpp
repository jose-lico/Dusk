#include "pch.h"
#include "Buffer.h"

#include "Core/Renderer/RendererContext.h"

#include "Platform/OpenGL/OpenGLVertexBuffer.h"
#include "Platform/OpenGL/OpenGLIndexBuffer.h"

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

    void VertexBufferLayout::Push(ShaderDataType type, unsigned int count, bool normalized)
    {
        m_Elements.push_back({ type, count, normalized });
        m_Stride += ShaderDataTypeSize(type) * count;
    }
}