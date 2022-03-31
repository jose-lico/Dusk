#pragma once
#include "Core/Macros/DUSK_API.h"

#include "Utils/Memory/Memory.h"

namespace DuskEngine {
	
	class VertexArray;
	class Mesh;

	class DUSK_EXPORT Renderer
	{
	public:
		static void BeginScene();
		static void EndScene();

		static void Submit(const Ref<VertexArray>& vertexArray);
		static void Submit(const Ref<Mesh>& mesh);
	};
}