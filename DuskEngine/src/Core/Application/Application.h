#pragma once
#include "Core/Macros/DUSK_API.h"

#include "LayerStack.h"
#include "Core/Renderer/RendererContext.h"
#include "Utils/ImGui/ImGuiLayer.h"
#include "Core/Events/Event.h"

namespace DuskEngine
{
	class DUSK_EXPORT Application
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

	extern Application* CreateApplication();
}