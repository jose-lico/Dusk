#include "pch.h"

#include "VertexBufferLayout.h"

namespace DuskEngine
{
    template<>
    void VertexBufferLayout::Push<float>(unsigned int count)
    {
        m_Elements.push_back({ GL_FLOAT, count, false });
        m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
    }

    template<>
    void VertexBufferLayout::Push<int>(unsigned int count)
    {
        m_Elements.push_back({ GL_UNSIGNED_INT, count, false });
        m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
    }

    template<>
    void VertexBufferLayout::Push<unsigned char>(unsigned int count)
    {
        m_Elements.push_back({ GL_UNSIGNED_BYTE, count, false });
        m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
    }
}