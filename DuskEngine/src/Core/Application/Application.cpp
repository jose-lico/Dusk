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
#ifdef DUSK_IMGUI
#include "Utils/ImGui/ImGuiLayer.h"
#endif

namespace DuskEngine
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationSpecs& specs)
		:m_Specs(specs)
	{
		s_Instance = this;

		time_t now = time(0);
		struct tm tstruct;
		char buffer[80];
		tstruct = *localtime(&now);
		strftime(buffer, sizeof(buffer), "%Y-%m-%d_%Hh%Mm%Ss", &tstruct);

		m_StartupTime = buffer;

		m_OS = OS::Create();

		m_Logger = new Logger(LOGGER);

		WindowData data;
		data.Title = m_Specs.Name + " | " + m_Specs.Platform + " | " + m_Specs.Target;
		m_Window = new Window(data);
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		m_Renderer = new Renderer(*m_Window);

		m_AssetDatabase = new AssetDatabase();
		m_AssetDatabase->LoadProject();
		
		m_LayerStack = new LayerStack();
#ifdef DUSK_IMGUI
		m_ImGuiLayer = new ImGuiLayer(&GetWindow());
		m_LayerStack->PushOverlay(m_ImGuiLayer);
		m_ImGuiLayer->OnAttach();
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
			for (Layer* layer : *m_LayerStack)
				if (layer->Enabled)
					layer->OnUpdate();

#ifdef DUSK_IMGUI
			m_ImGuiLayer->Begin();
#endif
			for (Layer* layer : *m_LayerStack)
				if (layer->Enabled)
					layer->OnImGuiRender();
#ifdef DUSK_IMGUI
			m_ImGuiLayer->End();
#endif
			m_Renderer->SwapBuffers();
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
}