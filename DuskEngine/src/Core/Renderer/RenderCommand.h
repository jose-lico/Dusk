#pragma once
#include "Core/Macros/DUSK_API.h"

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
		static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray);
	private:
		static RendererAPI* s_RendererAPI;
	};
}