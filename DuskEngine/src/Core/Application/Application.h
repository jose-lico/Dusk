#pragma once
#include "Core.h"

#include <string>

namespace DuskEngine
{
	class Layer;
	class Event;
	class Window;
	class Logger;
	class WindowManager;
	class RendererContext;
	class LayerStack;
	class ImGuiLayer;

	struct ApplicationSpecs
	{
		std::string Name = "Dusk Application";
	};

	class DUSK_EXPORT Application
	{
	public:
		Application(const ApplicationSpecs& specs);
		virtual ~Application();

		void Run();
		
		static Application& Get() { return *s_Instance; }
		
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void OnEvent(Event& e);

		void SetName(const char* name);

		Window& GetWindow();
	private:
		static Application* s_Instance;

		Logger* m_Logger;
		WindowManager* m_WindowManager;
		RendererContext* m_RendererContext;
		
		LayerStack* m_LayerStack;
		ImGuiLayer* m_ImGuiLayer; // Deleted by m_LayerStack
	
		ApplicationSpecs m_Specs;
	};

	extern Application* CreateApplication();
}