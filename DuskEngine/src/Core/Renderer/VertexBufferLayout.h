#pragma once

#include "Core/Application/Macros.h"
#include "pch.h"

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
}