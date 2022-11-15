#pragma once

namespace DuskEngine 
{
	struct VertexArray;
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
	};
}