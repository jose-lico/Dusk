#include "pch.h"
#include "Application.h"

#include "Utils/Logging/Logger.h"
#include "Utils/Window/WindowManager.h"
#include "Core/Renderer/Renderer.h"
#include "Core/Renderer/RenderCommand.h"

#include "Utils/Logging/Log.h"
#include "Input.h"

#include "imgui.h"
#include "Time.h"

namespace DuskEngine
{
	Application::Application()
	{
		Init();
	}

	Application::~Application()
	{
		Shutdown();
	}

	void Application::Init()
	{
		// Initialize subsystems
		Logger::Init();
		WindowManager::Init();
		WindowManager::GetWindow()->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
		rendererContext = RendererContext::Create(WindowManager::GetWindow()->GetNativeHandle());
		rendererContext->Init();
		Renderer::Init();
		
		m_ImGuiLayer = new ImGuiLayer();
		m_LayerStack.PushOverlay(m_ImGuiLayer);
		m_ImGuiLayer->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		for (Layer* layer : m_LayerStack)
		{
			if (e.Handled)
				break;
			layer->OnEvent(e);
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushLayer(overlay);
		overlay->OnAttach();
	}
	
	void Application::Run()
	{
		while (!WindowManager::GetWindow()->ShouldClose())
		{
			Time::Update();
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			rendererContext->SwapBuffers();
		}
	}

	void Application::Shutdown()
	{
		DUSK_LOG_INFO("##### SHUTDOWN #####");

		// Shutdown subsystems
		Renderer::Shutdown();
		rendererContext->Shutdown();
		WindowManager::Shutdown();
		Logger::Shutdown();
	}
}