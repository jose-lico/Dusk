#pragma once
#include "Core/Macros/DUSK_API.h"

#include "Core/Resources/Resources/Mesh.h"

#include "Utils/Memory/Memory.h"

namespace DuskEngine {

	class DUSK_EXPORT Renderer
	{
	public:
		static void BeginScene();
		static void EndScene();

		static void Submit(const Ref<VertexArray>& vertexArray);
		static void Submit(const Ref<Mesh>& mesh);
	};
}