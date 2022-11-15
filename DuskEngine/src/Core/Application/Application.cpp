#include "pch.h"
#include "Application.h"

#include "Time.h"
#include "Layer.h"
#include "LayerStack.h"
#include "Window.h"
#include "OS.h"
#include "Core/Assets/AssetDatabase.h"
#include "Core/Events/EventBase.h"
#include "Core/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLAPI.h"
#include "Utils/Profiling/Timer.h"
#ifdef DUSK_IMGUI
#include "Utils/ImGui/ImGuiLayer.h"
#endif

#undef CreateWindow

namespace DuskEngine
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationSpecs& specs, const CliOptions& options)
		:m_Specs(specs), m_Options(options)
	{
		s_Instance = this;

		m_OS = OS::Create();

		if (m_OS->IsConsoleAttached())
		{
			printf("Dusk Engine WIP - https://github.com/jose-lico/Dusk\n");
			printf("Personal game engine to explore and learn\n");
			printf("(c) 2021-2022 Jose Lico\n\n");
		}

		if (m_Options.Help)
		{
			printf("Usage: %s [options]\n\n", m_Options.ExeName.c_str());
			printf("Options:\n\n");
			printf("-h, --help                              Print this message and quit the application\n");
			printf("-v, --verbose                           Prints more useful information to the console\n");
			printf("-l, --log                               Dumps all logs to a file. Example log file name: %s.csv\n", m_StartupTime.c_str());
			return;
		}
		
#if DUSK_DEBUG
		m_Options.Verbose = true;
		m_Options.DumpLogs = true;
#endif

		time_t now = time(0);
		struct tm tstruct;
		char buffer[80];
		tstruct = *localtime(&now);
		strftime(buffer, sizeof(buffer), "%Y-%m-%d_%Hh%Mm%Ss", &tstruct);

		m_StartupTime = buffer;

		m_Logger = new Logger(LOGGER);

		m_AssetDatabase = new AssetDatabase();
		
		m_LayerStack = new LayerStack();
#ifdef DUSK_IMGUI
		{
			//Timer imguiTimer("ImGuiLayer");
			m_ImGuiLayer = new ImGuiLayer();
			PushOverlay(m_ImGuiLayer);
		}
#endif
	}

	Application::~Application()
	{
		TRACE("########## SHUTDOWN ##########");

		delete m_AssetDatabase;
		delete m_Renderer;
		delete m_LayerStack;
		delete m_Window;
		delete m_Logger;
	}

	void Application::Run()
	{
		if (m_Options.Help)
			return;

		while (!m_Window->ShouldClose())
		{
			Time::Update();
			for (uint32_t i = 0; i < *m_LayerStack; i++)
			{
				Layer& layer = *(*m_LayerStack)[i];

				if (layer.Enabled)
					layer.OnUpdate();
			}

#ifdef DUSK_IMGUI
			m_ImGuiLayer->Begin();
			for (uint32_t i = 0; i < *m_LayerStack; i++)
			{
				Layer& layer = *(*m_LayerStack)[i];

				if (layer.Enabled)
					layer.OnImGuiRender();
			}
			m_ImGuiLayer->End();
#endif
			{
				//Timer timer("Swap buffers", true);
				OpenGLAPI::SwapBuffers(m_Window->GetNativeHandle());
			}
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack->PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack->PushLayer(overlay);
		overlay->OnAttach();
	}

	void Application::PopLayer(Layer* layer)
	{
		m_LayerStack->PopLayer(layer);
		layer->OnDetach();
	}

	void Application::PopOverlay(Layer* overlay)
	{
		m_LayerStack->PopOverlay(overlay);
		overlay->OnDetach();
	}

	void Application::OnEvent(Event& e)
	{
		for (uint32_t i = 0; i < *m_LayerStack; i++)
		{
			if (e.Handled)
				break;

			Layer& layer = *(*m_LayerStack)[i];

			if (layer.Enabled)
				layer.OnEvent(e);
		}
	}

	void Application::SetImGuiGLContext() const
	{
		m_ImGuiLayer->SetGLContext(m_Window);
	}

	void Application::DestroyImGuiGLContext() const
	{
		m_ImGuiLayer->DestroyGLContext();
	}

	Window& Application::CreateWindow(WindowData& data)
	{
		//WindowData data;
		/*data.Title = m_Specs.Name + " | " + m_Specs.Platform + " | " + m_Specs.Target;
		data.Title = "Editor";*/

		m_Window = new Window(data);
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		return *m_Window;
	}
}