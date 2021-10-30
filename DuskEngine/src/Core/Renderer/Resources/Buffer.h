#pragma once
#include "../../Macros/DUSK_API.h"

#include <memory>
#include <vector>

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

	class DUSK_API VertexBufferLayout
	{
	public:
		VertexBufferLayout();
		~VertexBufferLayout();
		void Push(ShaderDataType type, unsigned int count, bool normalized);

		const std::vector<DuskEngine::VertexBufferElement> GetElements() const;
		unsigned int GetStride() const;
	private:
		class VblImpl;
		VblImpl* m_Impl;
	};

	class DUSK_API VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const std::shared_ptr<VertexBufferLayout>& GetLayout() const = 0;
		virtual void SetLayout(const std::shared_ptr<VertexBufferLayout>& layout) = 0;

		static VertexBuffer* Create(const void* data, int size);
	};

	class DUSK_API IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual unsigned int GetCount() const = 0;

		static IndexBuffer* Create(const void* data, int count);
	};
}