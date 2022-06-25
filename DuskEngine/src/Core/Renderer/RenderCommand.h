#pragma once
#include "Core/Application/Core.h"

#include "glm/glm.hpp"

namespace DuskEngine {
	
	class VertexArray;
	class RendererAPI;

	class DUSK_EXPORT RenderCommand
	{
	public:
		static void Init();
		static void Shutdown();
		static void SetClearColor(const glm::vec4& color);
		static void Clear();
		static void UnbindVAO();
		static void DrawIndexed(const UniqueRef<VertexArray>& vertexArray);
		static void DrawArrays(unsigned int start, unsigned int count);
	private:
		static RendererAPI* s_RendererAPI;
	};
}