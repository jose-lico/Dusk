#pragma once
#include <glm.hpp>

#include "VertexArray.h"

namespace DuskEngine {

	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1, D3D11 = 2
		};
	public:
		virtual void Init() = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;

		static API GetAPI() { return s_API; }

		static RendererAPI* Create();
	private:
		static API s_API;
	};

}