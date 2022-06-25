#pragma once
#include "Core/Application/Core.h"

#include "Utils/Memory/Memory.h"

namespace DuskEngine {
	
	class VertexArray;
	class Mesh;

	class DUSK_EXPORT Renderer
	{
	public:
		static void BeginScene();
		static void EndScene();

		static void Submit(const UniqueRef<VertexArray>& vertexArray);
		static void Submit(const Ref<Mesh>& mesh);
	};
}