#pragma once

#include "Utils/Memory/Memory.h"

#include "glm/glm.hpp"

namespace DuskEngine
{
	class VertexArray;
	struct Texture;
	enum class BufferType : uint16_t;
	enum class UsageType : uint16_t;
}

namespace DuskEngine :: OpenGLAPI
{
	void Init();
	void SetClearColor(const glm::vec4& color);
	void Clear();

	void SetBufferData(uint32_t& id, BufferType buffer, UsageType usage, void* data, size_t size);
	void BindBuffer(BufferType BufferType, uint32_t id);
	void FreeBuffer(uint32_t& id);

	void SetTextureData(Texture& textureData, const unsigned char* data, bool offset = false);
	void BindTexture(uint32_t slot, uint32_t id);
	void FreeTexture(uint32_t& id);

	void UnbindVAO();
	void DrawIndexed(const VertexArray& vertexArray);
	void DrawArrays(unsigned int start, unsigned int count);
}