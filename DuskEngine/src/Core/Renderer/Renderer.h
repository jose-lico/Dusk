#pragma once

#include "RenderCommand.h"

namespace DuskEngine {

	class Renderer
	{
	public:
		void Init();
		void Shutdown();
		static void BeginScene();
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);
	};
}