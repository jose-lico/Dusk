#pragma once

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

		const std::vector<VertexBufferElement> GetElements() const { return m_Elements; }
		const unsigned int GetStride() const { return m_Stride; }
	private:
		int m_Stride;
		std::vector<VertexBufferElement> m_Elements;
	};

	class VertexBuffer
	{
	public:
		VertexBuffer(const void* data, unsigned int size);
		~VertexBuffer();

		void Bind() const;
		void Unbind() const;

		inline const UniqueRef<VertexBufferLayout>& GetLayout() const { return m_Layout; }
		inline void SetLayout(UniqueRef<VertexBufferLayout>& layout) { m_Layout = std::move(layout); }
	private:
		unsigned int m_ID;
		UniqueRef<VertexBufferLayout> m_Layout;
	};

	class IndexBuffer
	{
	public:
		IndexBuffer(const void* data, unsigned int count);
		~IndexBuffer();

		void Bind() const;
		void Unbind() const;

		inline unsigned int GetCount() const { return m_Count; }
	private:
		unsigned int m_ID;
		unsigned int m_Count;
	};
}