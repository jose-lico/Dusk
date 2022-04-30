#include "pch.h"
#include "Application.h"

#include "Time.h"
#include "LayerStack.h"

#include "Core/Macros/BIND_EVENT_FN.h"
#include "Core/Macros/LOG.h"

#include "Core/Resources/ResourceManager.h"
#include "Core/Renderer/RenderCommand.h"
#include "Core/Renderer/RendererContext.h"
#include "Core/Events/EventBase.h"
#include "Core/Scripting/ScriptingEngine.h"

#include "Utils/Window/WindowManager.h"
#include "Utils/Window/Window.h"
#include "Utils/ImGui/ImGuiLayer.h"

namespace DuskEngine
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		s_Instance = this;

		CREATE_LOGGER // Rework in the future
		
		m_WindowManager = new WindowManager();
		m_WindowManager->GetWindow()->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		m_RendererContext = RendererContext::Create(m_WindowManager->GetWindow()->GetNativeHandle());
		
		RenderCommand::Init();

		ResourceManager::Init();
		ResourceManager::LoadUUIDs();
		ScriptingEngine::Init();

		m_ImGuiLayer = new ImGuiLayer(&GetWindow());
		m_LayerStack = new LayerStack();
		m_LayerStack->PushOverlay(m_ImGuiLayer);
		m_ImGuiLayer->OnAttach();
	}

	Application::~Application()
	{
		TRACE("##### SHUTDOWN #####");

		ScriptingEngine::Shutdown();
		ResourceManager::Shutdown();
		delete m_RendererContext;
		delete m_LayerStack; // Deletes m_ImGuiLayer
		delete m_WindowManager;
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

	void Application::Run()
	{
		while (!m_WindowManager->GetWindow()->ShouldClose())
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

			m_RendererContext->SwapBuffers();
		}
	}

	Window& Application::GetWindow()
	{
		return *m_WindowManager->GetWindow();
	}
}