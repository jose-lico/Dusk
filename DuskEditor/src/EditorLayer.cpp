#include "EditorLayer.h"

#include "Core/Renderer/Resources/Framebuffer.h"
#include "Core/Serialization/SceneSerializer.h"
#include "Core/ECS/EditorCamera.h"
#include "Core/Scripting/LuaScript.h"
#include "Core/Assets/AssetManager.h"
#include "Core/Assets/AssetHandler.h"

#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"
#include "IconsForkAwesome.h"

namespace DuskEngine
{
	EditorLayer::EditorLayer()
	{
		CREATE_APP_LOGGER
		APP_TRACE("Tracing from the app")
		APP_LOG("Logging from the app")
	}

	EditorLayer::~EditorLayer()
	{
		for (Panel* panel : m_Panels)
		{
			delete panel;
		}
	}

	void EditorLayer::OnAttach()
	{
		m_AssetHandler = MakeRef<AssetHandler>("EditorHandler");

		m_EditorCamera = new EditorCamera();
		m_EditorCamera->transform.position = glm::vec3(-4.0f, 2.0f, 4.0f);
		m_EditorCamera->transform.rotation = glm::vec3(-0.25f, -0.9f, 0.0f);
		m_EditorCamera->camera.projectionMatrix = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.01f, 100.0f);

		FramebufferSpecification fbSpec;
		fbSpec.Width = 720;
		fbSpec.Height = 480;
		m_EditorSceneFB.reset(FrameBuffer::Create(fbSpec));
		m_PlayingSceneFB.reset(FrameBuffer::Create(fbSpec));
		m_EditingScene = MakeRef<Scene>();

		SceneSerializer::DeserializeText(m_EditingScene, "res/scenes/scene.yaml");

		m_Panels.push_back(new InspectorPanel(m_AssetHandler));
		InspectorPanel& inspector = *(InspectorPanel*)m_Panels.back();
		m_Panels.push_back(new ConsolePanel());
		m_Panels.push_back(new ContentBrowserPanel());
		m_Panels.push_back(new GameViewportPanel(m_PlayingSceneFB, *m_EditingScene->GetMainCamera(), &m_Playing));
		m_GameViewportPanel = (GameViewportPanel*)m_Panels.back();
		m_Panels.push_back(new SceneViewportPanel(m_EditorSceneFB, m_EditorCamera));
		m_SceneViewportPanel = (SceneViewportPanel*)m_Panels.back();
		m_Panels.push_back(new HierarchyPanel(m_EditingScene, inspector, *m_SceneViewportPanel));
		m_HierarchyPanel = (HierarchyPanel*)m_Panels.back();


		auto play = [&]() {
			m_Playing = true;
			m_PlayingScene = MakeRef<Scene>();
			SceneSerializer::DeserializeText(m_PlayingScene, "res/scenes/scene.yaml");
			m_PlayingScene->OnAwakeRuntime();
			ImGui::SetWindowFocus(ICON_FK_GAMEPAD "  Game");
			m_GameViewportPanel->SetCamera(m_PlayingScene->GetMainCamera());
			m_HierarchyPanel->SetScene(m_PlayingScene); 
		};

		auto stop = [&]() {
			m_PlayingScene->OnShutdownRuntime();
			m_PlayingScene = MakeRef<Scene>();
			m_Playing = false;
			m_Paused = false;
			ImGui::SetWindowFocus(ICON_FK_EYE "  Viewport");
			m_GameViewportPanel->SetCamera(m_EditingScene->GetMainCamera());
			m_HierarchyPanel->SetScene(m_EditingScene);
		};

		auto pause = [&]() {
			m_Paused = !m_Paused;
		};

		m_Panels.push_back(new Toolbar(&m_Playing, &m_Paused, play, stop, pause));
	}

	void EditorLayer::OnUpdate()
	{
		if (!m_Playing)
		{
			m_EditorSceneFB->Bind();
			m_EditingScene->OnUpdateEditor(*m_EditorCamera);
			m_EditorSceneFB->Unbind();

			m_PlayingSceneFB->Bind();
			m_EditingScene->OnUpdateRuntime(false);
			m_PlayingSceneFB->Unbind();
		}
		else
		{
			m_EditorSceneFB->Bind();
			m_PlayingScene->OnUpdateEditor(*m_EditorCamera);
			m_EditorSceneFB->Unbind();

			m_PlayingSceneFB->Bind();
			m_PlayingScene->OnUpdateRuntime(true, m_Paused);
			m_PlayingSceneFB->Unbind();
		}
	}

	void EditorLayer::OnImGuiRender()
	{
		m_Dockspace.BeginDockspace();

		for (Panel* panel : m_Panels)
		{
			panel->OnImGuiRender();
		}

		ImGui::Begin("Stuff");

		if (ImGui::Button("Save Scene"))
			SceneSerializer::SerializeText(m_EditingScene, "res/scenes/scene.yaml");

		ImGui::End();

		m_Dockspace.EndDockspace();
	}

	void EditorLayer::OnEvent(Event& event)
	{
		for (Panel* panel : m_Panels)
		{
			panel->OnEvent(event);
		}
	}
}