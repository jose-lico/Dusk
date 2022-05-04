#pragma once
#include "Core/Renderer/RendererAPI.h"

#include "Utils/Memory/Memory.h"

namespace DuskEngine {

	class VertexArray;

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
		virtual void UnbindVAO() override;
		virtual void DrawArrays(unsigned int start, unsigned int count) override;
	};
}