#pragma once

#ifdef DUSK_DEBUG
	#define TARGET "Debug" 
#else
	#define TARGET "Release" 
#endif

#ifdef DUSK_WINDOWS
	#define PLATFORM "Windows" 
#elif DUSK_LINUX
	#define PLATFORM "Linux" 
#endif

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
	class AssetDatabase;

	struct ApplicationSpecs
	{
		std::string Name;
		std::string Target = TARGET;
		std::string Platform = PLATFORM;
	};

	class Application
	{
	public:
		Application(const ApplicationSpecs& specs);
		virtual ~Application();

		void Run();
		
		static Application& Get() { return *s_Instance; }
		
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void OnEvent(Event& e);

		inline Window& GetWindow() const { return *m_Window; }
		inline Renderer& GetRenderer() const { return *m_Renderer; }
		inline AssetDatabase& GetAssetDatabase() const { return *m_AssetDatabase; }
	private:
		static Application* s_Instance;

#ifdef DUSK_IMGUI
		ImGuiLayer* m_ImGuiLayer;
#endif
		LayerStack* m_LayerStack;
		Logger* m_Logger;
		Window* m_Window;
		Renderer* m_Renderer;
		AssetDatabase* m_AssetDatabase;

		ApplicationSpecs m_Specs;
	};

	extern Application* CreateApplication();
}