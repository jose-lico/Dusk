#include "pch.h"
#include "Application.h"

#include "Time.h"
#include "LayerStack.h"

#include "Core/Assets/AssetManager.h"
#include "Core/Renderer/RenderCommand.h"
#include "Core/Renderer/RendererContext.h"
#include "Core/Events/EventBase.h"

#include "Utils/Window/WindowManager.h"
#include "Utils/Window/Window.h"
#include "Utils/ImGui/ImGuiLayer.h"

#include "Core.h"

namespace DuskEngine
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationSpecs& specs)
		:m_Specs(specs)
	{
		s_Instance = this;

		m_Logger = new Logger(LOGGER);
		TRACE("Engine Logger Created");
		TRACE("Second Trace");

		m_WindowManager = new WindowManager();
		m_WindowManager->GetWindow()->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		m_RendererContext = RendererContext::Create(m_WindowManager->GetWindow()->GetNativeHandle());
		
		RenderCommand::Init();

		AssetManager::Init();
		AssetManager::CreateUUIDs();
		AssetManager::LoadUUIDs();

		m_ImGuiLayer = new ImGuiLayer(&GetWindow());
		m_LayerStack = new LayerStack();
		m_LayerStack->PushOverlay(m_ImGuiLayer);
		m_ImGuiLayer->OnAttach();
	}

	Application::~Application()
	{
		TRACE("########## SHUTDOWN ##########");

		AssetManager::Shutdown();
		delete m_RendererContext;
		delete m_LayerStack; // Deletes m_ImGuiLayer
		delete m_WindowManager;
		delete m_Logger;
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