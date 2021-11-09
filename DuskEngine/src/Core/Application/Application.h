#pragma once
#include "Core/Macros/DUSK_API.h"

#include "Utils/Memory/Memory.h"

#include "Core/Events/Event.h"
#include "LayerStack.h"
#include "Core/Renderer/RendererContext.h"
#include "Utils/ImGui/ImGuiLayer.h"

namespace DuskEngine
{
	class DUSK_EXPORT Application
	{
	public:
		Application();
		virtual ~Application();

		void Init();
		void Shutdown();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		
		void OnEvent(Event& e);
		
		void Run();
	private:
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer; // deleted by layerstack
		UniqueRef<RendererContext> rendererContext;
	};

	extern Application* CreateApplication();
}