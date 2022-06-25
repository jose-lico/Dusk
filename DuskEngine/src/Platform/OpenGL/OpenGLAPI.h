#pragma once

#include "Utils/Memory/Memory.h"

#include "glm/glm.hpp"

namespace DuskEngine
{
	class VertexArray;
}

namespace DuskEngine :: OpenGLAPI
{
	void Init();
	void SetClearColor(const glm::vec4& color);
	void Clear();

	void UnbindVAO();
	void DrawIndexed(const UniqueRef<DuskEngine::VertexArray>& vertexArray);
	void DrawArrays(unsigned int start, unsigned int count);
}