#pragma once

#include "defines.h"
#include "GL/glew.h"
#include "Utils/Window/Window.h"
#include "Platform/OpenGL/Shader.h"
#include "Platform/OpenGL/Texture.h"
#include "Platform/OpenGL/Framebuffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Camera.h"

// TESTING
#include "Utils/Logging/LoggingManager.h"
#include "Utils/Window/WindowManager.h"
#include "Renderer/RendererContext.h"
#include "Renderer/Renderer.h"

namespace DuskEngine
{
	class DUSK_API Application
	{
	public:
		Application();
		virtual ~Application() {};

		void Init();
		void Run();
		void Shutdown();
	private:
		std::shared_ptr<VertexArray> m_VA;
		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<Texture> m_Texture;

		Camera* m_Camera;

		// TESTING
		Logger* logger;
		WindowManager* windowManager;
		RendererContext* contextManager;
		Renderer* renderer;
	};
}