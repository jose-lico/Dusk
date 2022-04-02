#pragma once
#include "Core/Macros/DUSK_API.h"

namespace DuskEngine
{
	class Layer;
	class Event;
	class Window;
	class LayerStack;
	class ImGuiLayer;
	class WindowManager;
	class RendererContext;

	class DUSK_EXPORT Application
	{
	public:
		Application();
		virtual ~Application();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		
		void OnEvent(Event& e);
		void Run();

		Window& GetWindow();

		static Application& Get() { return *s_Instance; }
	private:
		static Application* s_Instance;

		LayerStack* m_LayerStack;
		ImGuiLayer* m_ImGuiLayer; // Deleted by m_LayerStack
		WindowManager* m_WindowManager;
		RendererContext* m_RendererContext;
	};

	extern Application* CreateApplication();
}