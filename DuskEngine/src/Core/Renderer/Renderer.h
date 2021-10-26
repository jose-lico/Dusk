#pragma once
#include "Core/Macros/DUSK_API.h"

#include "RenderCommand.h"
#include "Resources/Mesh.h"

#include <memory>

namespace DuskEngine {

	class DUSK_API Renderer
	{
	public:
		static void Init();
		static void Shutdown();
		static void BeginScene();
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);
		static void Submit(const std::shared_ptr<Mesh>& mesh);
	};
}