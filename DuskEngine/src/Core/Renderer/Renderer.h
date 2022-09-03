#pragma once

#include "Utils/Memory/Memory.h"

namespace DuskEngine 
{
	class OpenGLContext;
	class VertexArray;
	class Mesh;
	class Window;

	class Renderer
	{
	public:
		Renderer(const Window& window);
		~Renderer();

		void BeginScene();
		void EndScene();

		void Submit(const UniqueRef<VertexArray>& vertexArray);
		void Submit(const UniqueRef<Mesh>& mesh);

		void SwapBuffers();
	private:
		UniqueRef<OpenGLContext> m_Context;
	};
}