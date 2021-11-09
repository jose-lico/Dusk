#pragma once
#include "Core/Macros/DUSK_API.h"

#include "Utils/Memory/Memory.h"

#include "Resources/Mesh.h"

namespace DuskEngine {

	class DUSK_EXPORT Renderer
	{
	public:
		static void Init();
		static void Shutdown();
		static void BeginScene();
		static void EndScene();

		static void Submit(const Ref<VertexArray>& vertexArray);
		static void Submit(const Ref<Mesh>& mesh);
	};
}