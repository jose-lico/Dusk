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
#include "Utils/Profiling/Timer.h"
#ifdef DUSK_IMGUI
#include "Utils/ImGui/ImGuiLayer.h"
#endif

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

		WindowData data;
		//data.Title = m_Specs.Name + " | " + m_Specs.Platform + " | " + m_Specs.Target;
		data.Title = "Launcher";

		m_Window = new Window(data);
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
		m_Renderer = new Renderer(*m_Window);

		m_AssetDatabase = new AssetDatabase();
		
		m_LayerStack = new LayerStack();
#ifdef DUSK_IMGUI
		{
			//Timer imguiTimer("ImGuiLayer");
			m_ImGuiLayer = new ImGuiLayer(&GetWindow());
			PushOverlay(m_ImGuiLayer);
		}
#endif
	}

	Application::~Application()
	{
		TRACE("########## SHUTDOWN ##########");

		delete m_AssetDatabase;
		delete m_Renderer;
		delete m_LayerStack; // Deletes m_ImGuiLayer
		delete m_Window;
		delete m_Logger;
	}

	void Application::Run()
	{
		while (!m_Window->ShouldClose())
		{
			Time::Update();
			for (size_t i = 0; i < *m_LayerStack; i++)
			{
				if((*m_LayerStack)[i]->Enabled)
					(*m_LayerStack)[i]->OnUpdate();
			}

#ifdef DUSK_IMGUI
			m_ImGuiLayer->Begin();
			for (size_t i = 0; i < *m_LayerStack; i++)
			{
				if ((*m_LayerStack)[i]->Enabled)
					(*m_LayerStack)[i]->OnImGuiRender();
			}
			m_ImGuiLayer->End();
#endif
			{
				//Timer timer("Swap buffers", true);
				m_Renderer->SwapBuffers();
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
		for (Layer* layer : *m_LayerStack)
		{
			if (e.Handled)
				break;
			if(layer->Enabled)
				layer->OnEvent(e);
		}
	}

	void Application::CreateWindowDusk()
	{
		WindowData data;
		data.Title = m_Specs.Name + " | " + m_Specs.Platform + " | " + m_Specs.Target;
		data.Title = "Editor";

		m_ImGuiLayer->OnDetach();

		delete m_Window;
		delete m_Renderer;

		m_Window = new Window(data);
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
		m_Renderer = new Renderer(*m_Window);

		m_ImGuiLayer->SetGLContext(m_Window);
	}
}