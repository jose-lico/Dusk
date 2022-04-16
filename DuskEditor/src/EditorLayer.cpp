#include "EditorLayer.h"

#include "Core/Renderer/Resources/Framebuffer.h"
#include "Core/Serialization/SceneSerializer.h"

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
		FramebufferSpecification fbSpec;
		fbSpec.Width = 720;
		fbSpec.Height = 480;
		m_FB.reset(FrameBuffer::Create(fbSpec));
		m_EditingScene = MakeRef<Scene>();

		SceneSerializer::DeserializeText(m_EditingScene, "res/scenes/scene.yaml");

		m_Panels.push_back(new InspectorPanel());
		InspectorPanel& inspector = *(InspectorPanel*)m_Panels.back();
		m_Panels.push_back(new ConsolePanel());
		m_Panels.push_back(new ContentBrowserPanel());
		m_Panels.push_back(new GameViewPortPanel(m_FB, *m_EditingScene->GetMainCamera(), &m_Playing));
		m_Panels.push_back(new SceneViewportPanel(m_FB, *m_EditingScene->GetMainCamera()));
		SceneViewportPanel& viewport = *(SceneViewportPanel*)m_Panels.back();
		m_Panels.push_back(new HierarchyPanel(m_EditingScene, inspector, viewport));

		auto ent = m_EditingScene->FindEntity("Lit Cube");
	}

	void EditorLayer::OnUpdate()
	{
		m_FB->Bind();

		m_EditingScene->OnUpdate();

		m_FB->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		m_Dockspace.BeginDockspace();

		ImGui::Begin("Stuff");

		if (ImGui::Button("Save Scene"))
			SceneSerializer::SerializeText(m_EditingScene, "res/scenes/scene.yaml");

		if (ImGui::Button("Play"))
		{
			if (!m_Playing)
			{
				m_Playing = true;
				m_PlayingScene = MakeRef<Scene>();
				SceneSerializer::DeserializeText(m_PlayingScene, "res/scenes/scene.yaml");
				ImGui::SetWindowFocus("Game");
			}
			else
			{
				m_PlayingScene = MakeRef<Scene>();
				m_Playing = false;
				ImGui::SetWindowFocus(ICON_FK_EYE "  Viewport");
			}
		}

		ImGui::End();

		for (Panel* panel : m_Panels)
		{
			panel->OnImGuiRender();
		}

		m_Dockspace.EndDockspace();
	}
}