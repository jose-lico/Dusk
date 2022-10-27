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

		void Submit(const VertexArray& vertexArray);
		void Submit(const Mesh& mesh);

		void SwapBuffers();
	private:
		UniqueRef<OpenGLContext> m_Context;
	};
}