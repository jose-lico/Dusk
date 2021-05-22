#pragma once

#include "defines.h"
#include "Window.h"
#include "Platform/OpenGL/Renderer.h" 
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
		bool m_Running = true;
		Window* m_Window;

		// TEMP
		Renderer& m_Renderer = Renderer::Get();

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