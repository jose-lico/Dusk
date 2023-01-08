#pragma once

#include "Resource.h"

#include <vector>

namespace DuskEngine
{
	enum class ShaderDataType : uint8_t;
	static unsigned int ShaderDataTypeSize(ShaderDataType type);

	enum class UsageType : uint16_t
	{
		STREAM_DRAW = 0x88E0,
		STREAM_READ,
		STREAM_COPY,
		STATIC_DRAW = 0x88E4,
		STATIC_READ,
		STATIC_COPY,
		DYNAMIC_DRAW = 0x88E8,
		DYNAMIC_READ,
		DYNAMIC_COPY
	};

	enum class BufferType : uint16_t
	{
		ARRAY_BUFFER = 0x8892,
		ELEMENT_ARRAY_BUFFER
	};

	enum class ShaderDataType : uint8_t
	{
		None = 0, 
		FLOAT, 
		FLOAT2,
		FLOAT3,
		FLOAT4,
		MAT3, 
		MAT4,
		INT, 
		INT2,
		INT3,
		INT4,
		BOOL
	};

	struct OpenGLBuffer : OpenGLResource
	{
		size_t Size;
		BufferType BufferType;
		UsageType UsageType;
	};

	struct IndexBuffer : OpenGLBuffer
	{
		uint32_t Count;
		size_t UnitSize;
	};

	struct VertexBufferElement
	{
		ShaderDataType ShaderType;
		uint8_t Count;
		bool Normalized;

		VertexBufferElement(ShaderDataType type, uint8_t count, bool normalized)
			: ShaderType(type), Count(count), Normalized(normalized) {}
	};

	struct VertexBufferLayout
	{
		uint32_t Stride = 0;
		std::vector<VertexBufferElement> Elements;

		void Push(ShaderDataType type, uint8_t count, bool normalized)
		{
			Elements.push_back({ type, count, normalized });
			Stride += ShaderDataTypeSize(type) * count;
		}
	};

	struct VertexBuffer : OpenGLBuffer
	{
		VertexBufferLayout Layout;
	};

	unsigned int ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::FLOAT:    return 4;
		case ShaderDataType::FLOAT2:   return 4 * 2;
		case ShaderDataType::FLOAT3:   return 4 * 3;
		case ShaderDataType::FLOAT4:   return 4 * 4;
		case ShaderDataType::MAT3:     return 4 * 3 * 3;
		case ShaderDataType::MAT4:     return 4 * 4 * 4;
		case ShaderDataType::INT:      return 4;
		case ShaderDataType::INT2:     return 4 * 2;
		case ShaderDataType::INT3:     return 4 * 3;
		case ShaderDataType::INT4:     return 4 * 4;
		case ShaderDataType::BOOL:     return 1;
		}

		return 0;
	}
}
