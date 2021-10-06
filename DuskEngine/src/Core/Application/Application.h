#pragma once

#include "Macros.h"

#include "Core/Renderer/RendererContext.h"
#include "Platform/OpenGL/Shader.h"
#include "Platform/OpenGL/Texture.h"
#include "Core/Renderer/VertexArray.h"
#include "Core/Renderer/Camera.h"
#include "Utils/ImGui/ImGuiLayer.h"
#include "LayerStack.h"
#include "Core/Events/Event.h"

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
		void OnEvent(Event& e);
	private:
		RendererContext* rendererContext;
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;

		std::shared_ptr<VertexArray> m_VA;
		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<Texture> m_Texture;
		Camera* m_Camera;
	};
}