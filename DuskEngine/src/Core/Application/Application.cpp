#include "pch.h"
#include "Application.h"

#include "Time.h"
#include "LayerStack.h"
#include "Window.h"

#include "Core/Assets/AssetDatabase.h"
#include "Core/Events/EventBase.h"
#include "Core/Renderer/Renderer.h"

#include "Utils/ImGui/ImGuiLayer.h"

namespace DuskEngine
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationSpecs& specs)
		:m_Specs(specs)
	{
		s_Instance = this;

		m_Logger = new Logger(LOGGER);

		m_Window = new Window();
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
		m_Window->SetWindowTitle(m_Specs.Name.c_str());

		m_Renderer = new Renderer(*m_Window);

		AssetDatabase::Init();
		AssetDatabase::CreateUUIDs();
		AssetDatabase::LoadUUIDs();

		m_ImGuiLayer = new ImGuiLayer(&GetWindow());
		m_LayerStack = new LayerStack();
		m_LayerStack->PushOverlay(m_ImGuiLayer);
		m_ImGuiLayer->OnAttach();
	}

	Application::~Application()
	{
		TRACE("########## SHUTDOWN ##########");

		AssetDatabase::Shutdown();
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

			m_ImGuiLayer->Begin();
			for (Layer* layer : *m_LayerStack)
				if (layer->Enabled)
					layer->OnImGuiRender();
			m_ImGuiLayer->End();

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

	void Application::SetName(const char* name)
	{
		m_Specs.Name = name;
		m_Window->SetWindowTitle(name);
	}
}