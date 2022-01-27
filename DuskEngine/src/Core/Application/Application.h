#pragma once
#include "Core/Macros/DUSK_API.h"

#include "Utils/Memory/Memory.h"

#include "Core/Events/Event.h"
#include "LayerStack.h"
#include "Core/Renderer/RendererContext.h"
#include "Utils/ImGui/ImGuiLayer.h"
#include "Utils/Window/Window.h"

namespace DuskEngine
{
	class Window;
	class WindowManager;
	class RendererContext;
	class ResourceManager;

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
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer; // deleted by layerstack
		

		WindowManager* m_WindowManager;
		RendererContext* m_RendererContext;

		/* In the future:
		* FileManager
		*/
	};

	extern Application* CreateApplication();
}