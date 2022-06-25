#pragma once
#include "Core.h"

#include <string>

namespace DuskEngine
{
	class Layer;
	class Event;
	class Window;
	class Logger;
	class Renderer;
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

		Window& GetWindow() const { return *m_Window; }
		Renderer& GetRenderer() const { return *m_Renderer; }
	private:
		static Application* s_Instance;

		LayerStack* m_LayerStack;
		Logger* m_Logger;
		Window* m_Window;
		Renderer* m_Renderer;
		
		ImGuiLayer* m_ImGuiLayer; // Deleted by m_LayerStack | Should consider moving to editor
	
		ApplicationSpecs m_Specs;
	};

	extern Application* CreateApplication();
}