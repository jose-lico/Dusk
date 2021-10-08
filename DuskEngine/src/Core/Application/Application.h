#pragma once

#include "GL/glew.h"
#include "Macros.h"

#include "LayerStack.h"
#include "Core/Renderer/RendererContext.h"
#include "Utils/ImGui/ImGuiLayer.h"
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