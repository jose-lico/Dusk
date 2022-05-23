#pragma once
#include "Core.h"

#include <string>

namespace DuskEngine
{
	class Layer;
	class Event;
	class Window;
	class LayerStack;
	class ImGuiLayer;
	class WindowManager;
	class RendererContext;
	class Logger;

	// Add more stuff later
	struct ApplicationSpecs
	{
		//std::string Name = "Dusk Application";
	};

	class DUSK_EXPORT Application
	{
	public:
		Application(const ApplicationSpecs& specs);
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
	
		ApplicationSpecs m_Specs;
		Logger* m_Logger;
	};

	extern Application* CreateApplication();
}