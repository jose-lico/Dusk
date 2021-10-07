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

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
	private:
		LayerStack m_LayerStack;
		RendererContext* rendererContext;
		ImGuiLayer* m_ImGuiLayer;
	};

	Application* CreateApplication();
}