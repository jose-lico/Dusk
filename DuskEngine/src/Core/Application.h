#pragma once

#include "Macros.h"

#include "Renderer/RendererContext.h"
#include "Platform/OpenGL/Shader.h"
#include "Platform/OpenGL/Texture.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Camera.h"

namespace DuskEngine
{
	class DUSK_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Init();
		void Run();
		void Shutdown();
	private:
		RendererContext* rendererContext;

		std::shared_ptr<VertexArray> m_VA;
		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<Texture> m_Texture;

		Camera* m_Camera;
	};
}