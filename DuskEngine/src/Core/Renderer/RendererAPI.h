#pragma once

#include "glm/glm.hpp"

namespace DuskEngine {

	class VertexArray;

	class RendererAPI
	{ 
	public:
		virtual void Init() = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void UnbindVAO() = 0;
		virtual void DrawIndexed(const UniqueRef<VertexArray>& vertexArray) = 0;
		virtual void DrawArrays(unsigned int start, unsigned int count) = 0;

		static RendererAPI* Create();
	};

}