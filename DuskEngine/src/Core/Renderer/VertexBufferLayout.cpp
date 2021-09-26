#include "pch.h"
#include "VertexBufferLayout.h"

namespace DuskEngine
{
    VertexBufferLayout::VertexBufferLayout()
        :m_Stride(0)
    {
    }

    void VertexBufferLayout::Push(ShaderDataType type, unsigned int count, bool normalized)
    {
        m_Elements.push_back({ type, count, normalized });
        m_Stride += ShaderDataTypeSize(type) * count;
    }
}