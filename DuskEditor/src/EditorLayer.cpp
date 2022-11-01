#include "EditorLayer.h"

#include "Assets/AssetDatabaseEditor.h"

#include "Core/Application/Application.h"
#include "Core/Application/Window.h"
#include "Core/Serialization/SceneSerializer.h"
#include "Core/ECS/EditorCamera.h"
#include "Core/Scripting/LuaScript.h"
#include "Core/Assets/AssetDatabase.h"
#include "Core/Assets/AssetHandler.h"
#include "Platform/OpenGL/Framebuffer.h"
#include "Utils/Profiling/Timer.h"

#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"
#include "IconsForkAwesome.h"

namespace DuskEngine
{
	EditorLayer::EditorLayer()
	{
		m_Logger = new Logger("Editor");

		TRACE("Tracing from the editor");
		LOG("Logging from the editor");

		std::string title = Application::Get().GetWindow().GetTitle() + " | MyScene";
		Application::Get().GetWindow().SetWindowTitle(title);
	}

	EditorLayer::~EditorLayer()
	{
		for (Panel* panel : m_Panels)
		{
			delete panel;
		}

		delete(m_Logger);
	}

	void EditorLayer::OnAttach()
	{
		{
			Timer databaseTimer("Loading project assets");
			auto& db = Application::Get().GetAssetDatabase();

			m_EditorDB = new AssetDatabaseEditor(&db);
			m_EditorDB->RegisterAssets();
			m_EditorDB->ImportAssets();
		
			db.LoadProject();
		}

		m_AssetHandler = MakeRef<AssetHandler>("EditorHandler");

		m_EditorCamera = new EditorCamera();
		m_EditorCamera->transform.position = glm::vec3(-4.0f, 2.0f, 4.0f);
		m_EditorCamera->transform.rotation = glm::vec3(0.25f, 2.5f, 0.0f);
		m_EditorCamera->camera.projectionMatrix = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.01f, 100.0f);

		FramebufferSpecification fbSpec;
		fbSpec.Width = 720;
		fbSpec.Height = 480;
		m_EditorSceneFB = MakeRef<Framebuffer>(fbSpec);
		m_PlayingSceneFB = MakeRef<Framebuffer>(fbSpec);

		m_EditingScene = MakeRef<Scene>("Editing Scene");

		SceneSerializer::DeserializeText(m_EditingScene, "res/scenes/scene.yaml");

		m_Panels.push_back(new InspectorPanel(m_EditingScene->m_AssetHandler, m_EditorDB));
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

		
		//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		//ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 0,0 });
		//ImGui::Begin("Stuff");

		////ImGui::Indent();
		//float height = ImGui::GetContentRegionAvail().y;
		//ImGui::Image((ImTextureID)m_TestTexture->GetRendererID(), ImVec2{ 10, height }, ImVec2{ 0, 0 }, ImVec2{ 1, 1 }, 
		//	{ 0.12f, 0.12f, 0.12f, 1.0f });
		////ImGui::Text("Indented?");
		////ImGui::Unindent();
		////ImGui::Text("Nope?");

		////if (ImGui::Button("Save Scene"))
		////	SceneSerializer::SerializeText(m_EditingScene, "res/scenes/scene.yaml");

		///*ImGui::PushStyleColor(ImGuiCol_ChildBg, { 0.2, 0.2, 0.2, 1.0f });
		//ImGui::BeginChild("Window inside a window", {0,0}, false);
		//ImGui::EndChild();
		//ImGui::PopStyleColor();*/
		//
		//ImGui::End();
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