#include "EditorLayer.h"

#include "Assets/AssetDatabaseEditor.h"

#include "Core/Application/Application.h"
#include "Core/Application/Window.h"
#include "Core/Serialization/SceneSerializer.h"
#include "Core/ECS/EditorCamera.h"
#include "Core/Scripting/LuaScript.h"
#include "Core/Assets/AssetDatabase.h"
#include "Core/Assets/AssetHandler.h"
#include "Platform/OpenGL/OpenGLAPI.h"
#include "Utils/Profiling/Timer.h"

#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"
#include "IconsForkAwesome.h"

#include <cstdio>

namespace DuskEngine
{
	EditorLayer::EditorLayer()
	{
		m_Logger = new Logger(LOGGER);

		TRACE("Tracing from the editor");
		LOG("Logging from the editor");

		auto& app = Application::Get();

		WindowData data;
		const ApplicationSpecs& specs = app.GetSpecs();
		data.Title = specs.Name + " | " + specs.Platform + " | " + specs.Target + " | MyProject | MyScene";
		Window& window = app.CreateWindow(data);
		OpenGLAPI::CreateContext(window.GetNativeHandle());
		app.SetImGuiGLContext();
		Application::Get().GetWindow().Maximize();

		// On Load Project do something with path from clioptions
	}

	EditorLayer::~EditorLayer()
	{
		for (Panel* panel : m_Panels)
		{
			delete panel;
		}

		OpenGLAPI::FreeFramebuffer(m_EditorSceneFB);
		OpenGLAPI::FreeFramebuffer(m_PlayingSceneFB);

		delete(m_Logger);
	}

	void EditorLayer::OnAttach()
	{
		{
			Timer databaseTimer("Loading project assets");
			auto& db = Application::Get().GetAssetDatabase();

			m_EditorDB = new AssetDatabaseEditor(&db);

			{
				Timer registerAssets("Register Assets");
				m_EditorDB->RegisterAssets();
			}

			m_EditorDB->ImportAssets();
		
			db.LoadProject();
		}

		m_AssetHandler = MakeRef<AssetHandler>("EditorHandler");

		m_EditorCamera = new EditorCamera();
		m_EditorCamera->transform.position = glm::vec3(-4.0f, 2.0f, 4.0f);
		m_EditorCamera->transform.rotation = glm::vec3(0.25f, 2.5f, 0.0f);
		m_EditorCamera->camera.projectionMatrix = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.01f, 100.0f);

		{
			Timer panels("FB");

			Framebuffer specs;
			specs.Width = 720;
			specs.Height = 480;

			m_EditorSceneFB = specs;
			m_PlayingSceneFB = specs;

			OpenGLAPI::ResizeFramebuffer(m_EditorSceneFB);
			OpenGLAPI::ResizeFramebuffer(m_PlayingSceneFB);
		}

		m_EditingScene = MakeRef<Scene>("Editing Scene");

		SceneSerializer::DeserializeText(m_EditingScene, "res/scenes/scene.yaml");

		{
			Timer panels("Creating Panels");

			m_Panels.push_back(new InspectorPanel(m_EditingScene->m_AssetHandler, m_EditorDB));
			InspectorPanel& inspector = *(InspectorPanel*)m_Panels.back();
			m_Panels.push_back(new ConsolePanel());
			m_Panels.push_back(new ContentBrowserPanel());
			m_Panels.push_back(new GameViewportPanel(&m_PlayingSceneFB, *m_EditingScene->GetMainCamera(), &m_Playing));
			m_GameViewportPanel = (GameViewportPanel*)m_Panels.back();
			m_Panels.push_back(new SceneViewportPanel(&m_EditorSceneFB, m_EditorCamera));
			m_SceneViewportPanel = (SceneViewportPanel*)m_Panels.back();
			m_Panels.push_back(new HierarchyPanel(m_EditingScene, inspector, *m_SceneViewportPanel));
			m_HierarchyPanel = (HierarchyPanel*)m_Panels.back();


			auto play = [&]() {
				m_Playing = true;
				m_PlayingScene = MakeRef<Scene>("Playing Scene");
				SceneSerializer::DeserializeText(m_PlayingScene, "res/scenes/scene.yaml");
				m_PlayingScene->OnAwakeRuntime();
				ImGui::SetWindowFocus(ICON_FK_GAMEPAD "  Game");
				m_GameViewportPanel->SetCamera(m_PlayingScene->GetMainCamera());
				m_HierarchyPanel->SetScene(m_PlayingScene); 
			};

			auto stop = [&]() {
				m_PlayingScene->OnShutdownRuntime();
				m_PlayingScene.reset();
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
			m_Panels.push_back(new DebugPanel());
		}
	}

	void EditorLayer::OnUpdate()
	{
		if (!m_Playing)
		{
			OpenGLAPI::BindFramebuffer(m_EditorSceneFB);
			m_EditingScene->OnUpdateEditor(*m_EditorCamera);
			OpenGLAPI::UnbindFramebuffer();

			{
				Timer timer("Update editor mode", true);
				OpenGLAPI::BindFramebuffer(m_PlayingSceneFB);
				m_EditingScene->OnUpdateRuntime(false);
				OpenGLAPI::UnbindFramebuffer();
			}
		}
		else
		{
			OpenGLAPI::BindFramebuffer(m_EditorSceneFB);
			m_PlayingScene->OnUpdateEditor(*m_EditorCamera);
			OpenGLAPI::UnbindFramebuffer();

			{
				Timer timer("Update runtime mode", true);
				OpenGLAPI::BindFramebuffer(m_PlayingSceneFB);
				m_PlayingScene->OnUpdateRuntime(true, m_Paused);
				OpenGLAPI::UnbindFramebuffer();
			}
		}
	}

	void EditorLayer::OnImGuiRender()
	{
		m_Dockspace.BeginDockspace();

		for (Panel* panel : m_Panels)
		{
			panel->OnImGuiRender();
		}

		
		//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		//ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 0,0 });
		ImGui::Begin("Stuff");

		////ImGui::Indent();
		//float height = ImGui::GetContentRegionAvail().y;
		//ImGui::Image((ImTextureID)m_TestTexture->GetRendererID(), ImVec2{ 10, height }, ImVec2{ 0, 0 }, ImVec2{ 1, 1 }, 
		//	{ 0.12f, 0.12f, 0.12f, 1.0f });
		////ImGui::Text("Indented?");
		////ImGui::Unindent();
		////ImGui::Text("Nope?");

		if (ImGui::Button("Save Scene"))
			SceneSerializer::SerializeText(m_EditingScene, "res/scenes/scene.yaml");

		///*ImGui::PushStyleColor(ImGuiCol_ChildBg, { 0.2, 0.2, 0.2, 1.0f });
		//ImGui::BeginChild("Window inside a window", {0,0}, false);
		//ImGui::EndChild();
		//ImGui::PopStyleColor();*/
		//
		ImGui::End();
		//ImGui::PopStyleVar();
		//ImGui::PopStyleVar();

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