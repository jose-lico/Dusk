#pragma once

#include "defines.h"
#include "Window.h"
#include "Platform/OpenGL/VertexArray.h"
#include "Platform/OpenGL/Shader.h"
 
namespace DuskEngine
{
	class DUSK_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	private:
		bool m_Running = true;
		Window* m_Window;

		// TEMP
		VertexArray* m_VA;
		VertexBuffer* m_VB;
		IndexBuffer* m_IB;
		Shader* m_Shader;
	};
}