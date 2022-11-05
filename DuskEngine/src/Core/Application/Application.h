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
	class OS;

	struct ApplicationSpecs
	{
		std::string Name;
		std::string Target = TARGET;
		std::string Platform = PLATFORM;
	};

	struct CliOptions
	{
		std::string ExeName;
		bool Help = false;
		bool Verbose = false;
		bool DumpLogs = false;
	};

	class Application
	{
	public:
		Application(const ApplicationSpecs& specs, const CliOptions& options);
		virtual ~Application();

		static Application& Get() { return *s_Instance; }
		
		void Run();
		
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void OnEvent(Event& e);

		inline Window& GetWindow() const { return *m_Window; }
		inline Renderer& GetRenderer() const { return *m_Renderer; }
		inline AssetDatabase& GetAssetDatabase() const { return *m_AssetDatabase; }
		inline OS& GetOS() const { return *m_OS; }

		inline const CliOptions& GetCliOptions() const { return m_Options; }

		inline const std::string& GetStartupTime() const { return m_StartupTime; }
	private:
		static Application* s_Instance;

#ifdef DUSK_IMGUI
		ImGuiLayer* m_ImGuiLayer = nullptr;
#endif
		LayerStack* m_LayerStack = nullptr;
		Logger* m_Logger = nullptr;
		Window* m_Window = nullptr;
		Renderer* m_Renderer = nullptr;
		AssetDatabase* m_AssetDatabase = nullptr;
		OS* m_OS = nullptr;

		ApplicationSpecs m_Specs;
		CliOptions m_Options;

		std::string m_StartupTime;

		friend class DuskEditor;
	};

	extern Application* CreateApplication(int argc, char** argv);
}