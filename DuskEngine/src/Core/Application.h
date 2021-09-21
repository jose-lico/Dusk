#pragma once

#include "defines.h"
#include "Window.h"
#include "Platform/OpenGL/OpenGLRenderer.h" 
#include "Platform/OpenGL/Texture.h"
#include "Platform/OpenGL/Framebuffer.h"

namespace DuskEngine
{
	class DUSK_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	private:
		//bool m_Running = true; using window close for now
		Window* m_Window;

		// TEMP
		OpenGLRenderer& m_Renderer = OpenGLRenderer::Get();

		VertexArray* m_VA;
		VertexBuffer* m_VB;
		IndexBuffer* m_IB;
		Shader* m_Shader;
		Texture* m_Texture;

		Framebuffer* m_Framebuffer;
		VertexArray* m_VAFramebuffer;
		VertexBuffer* m_VBFramebuffer;
	};
}