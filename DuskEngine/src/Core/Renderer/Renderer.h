#pragma once

namespace DuskEngine 
{
	struct VertexArray;
	class OpenGLContext;
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
		OpenGLContext* m_Context;
	};
}