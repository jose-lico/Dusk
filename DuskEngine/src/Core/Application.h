#pragma once

#include "defines.h"
#include "Window.h"
#include "Platform/OpenGL/Shader.h"
#include "Platform/OpenGL/Texture.h"
#include "Platform/OpenGL/Framebuffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Camera.h"
#include "Utils/Logging/Logger.h"

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

		std::shared_ptr<VertexArray> m_VA;
		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<Texture> m_Texture;

		Framebuffer* m_Framebuffer;
		std::shared_ptr<VertexArray> m_VAFramebuffer;

		Camera m_Camera;

		Logger logger;
	};
}