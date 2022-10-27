#pragma once

#include "Core/Assets/Resource.h"

namespace DuskEngine
{
	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static unsigned int ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return 4;
		case ShaderDataType::Float2:   return 4 * 2;
		case ShaderDataType::Float3:   return 4 * 3;
		case ShaderDataType::Float4:   return 4 * 4;
		case ShaderDataType::Mat3:     return 4 * 3 * 3;
		case ShaderDataType::Mat4:     return 4 * 4 * 4;
		case ShaderDataType::Int:      return 4;
		case ShaderDataType::Int2:     return 4 * 2;
		case ShaderDataType::Int3:     return 4 * 3;
		case ShaderDataType::Int4:     return 4 * 4;
		case ShaderDataType::Bool:     return 1;
		}

		return 0;
	}

	struct VertexBufferElement
	{
		ShaderDataType type;
		unsigned int count;
		bool normalized;
	};

	class VertexBufferLayout
	{
	public:
		VertexBufferLayout() :m_Stride(0) {}
		~VertexBufferLayout() = default;

		void Push(ShaderDataType type, unsigned int count, bool normalized);

		inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
		inline const unsigned int GetStride() const { return m_Stride; }
	private:
		int m_Stride;
		std::vector<VertexBufferElement> m_Elements;
	};

	class VertexBuffer : public Resource
	{
	public:
		VertexBuffer();
		~VertexBuffer() = default;

		virtual void Free() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		void SetData(const void* data, unsigned int size);

		inline const VertexBufferLayout& GetLayout() const { return m_Layout; }
		inline void SetLayout(VertexBufferLayout& layout) { m_Layout = std::move(layout); }
	private:
		VertexBufferLayout m_Layout;
	};

	class IndexBuffer : public Resource
	{
	public:
		IndexBuffer();
		~IndexBuffer() = default;

		virtual void Free() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		void SetData(const void* data, unsigned int count);

		inline unsigned int GetCount() const { return m_Count; }
	private:
		unsigned int m_Count;
	};
}