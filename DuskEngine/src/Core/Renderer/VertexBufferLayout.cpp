#include "pch.h"
#include "VertexBufferLayout.h"

namespace DuskEngine
{
    class VertexBufferLayout::VblImpl
    {
    public:
        VblImpl() :m_Stride(0) {}

        void Push(ShaderDataType type, unsigned int count, bool normalized)
        {
            m_Elements.push_back({ type, count, normalized });
            m_Stride += ShaderDataTypeSize(type) * count;
        }

        const std::vector<DuskEngine::VertexBufferElement> GetElements() const { return m_Elements; }
        unsigned int GetStride() const { return m_Stride; }
    private:
        std::vector<VertexBufferElement> m_Elements;
        int m_Stride;
    };

    VertexBufferLayout::VertexBufferLayout()
        :m_Impl(new VblImpl())
    {
    }

    VertexBufferLayout::~VertexBufferLayout()
    {
        delete m_Impl;
    }

    void VertexBufferLayout::Push(ShaderDataType type, unsigned int count, bool normalized)
    {
        m_Impl->Push(type, count, normalized);
    }

    const std::vector<DuskEngine::VertexBufferElement> VertexBufferLayout::GetElements() const
    {
        return m_Impl->GetElements();
    }

    unsigned int VertexBufferLayout::GetStride() const
    {
        return m_Impl->GetStride();
    }
}