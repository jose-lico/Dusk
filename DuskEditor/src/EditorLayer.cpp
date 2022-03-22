#include "EditorLayer.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

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
		m_Scene = MakeRef<Scene>();

		SceneSerializer::DeserializeText(m_Scene, "res/scenes/scene.yaml");

		m_Panels.push_back(new InspectorPanel());
		InspectorPanel& inspector = *(InspectorPanel*)m_Panels.back();
		m_Panels.push_back(new SceneViewportPanel(m_FB, *m_Scene->GetMainCamera()));
		SceneViewportPanel& viewport = *(SceneViewportPanel*)m_Panels.back();
		m_Panels.push_back(new HierarchyPanel(m_Scene, inspector, viewport));
		m_Panels.push_back(new ConsolePanel());
		m_Panels.push_back(new ContentBrowserPanel());

		auto ent = m_Scene->FindEntity("Lit Cube");

		ent->AddComponent<TestScript>(ent);
	}

	void EditorLayer::OnUpdate()
	{
		m_FB->Bind();

		m_Scene->OnUpdate();

		m_FB->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		m_Dockspace.BeginDockspace();

		for (Panel* panel : m_Panels)
		{
			panel->OnImGuiRender();
		}

		ImGui::Begin("Save Scene");

		if(ImGui::Button("Save Scene"))
			SceneSerializer::SerializeText(m_Scene, "res/scenes/scene.yaml");

		ImGui::End();

		m_Dockspace.EndDockspace();
	}
}