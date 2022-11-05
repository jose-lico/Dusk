#pragma once
#include "Core/Application/Layer.h"

#include "Utils/Memory/Memory.h"

#include "Panels.h"

#include <vector>

namespace DuskEngine
{
	class Scene;
	class Framebuffer;
	class EditorCamera;
	class AssetDatabaseEditor;
	class Logger;

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		~EditorLayer();
		void OnAttach() override;
		void OnUpdate() override;
		void OnImGuiRender() override;
		void OnEvent(Event& event) override;
	private:
		Dockspace m_Dockspace;
		std::vector<Panel*> m_Panels;

		Ref<Scene> m_EditingScene;
		Ref<Scene> m_PlayingScene;

		Ref<Framebuffer> m_EditorSceneFB;
		Ref<Framebuffer> m_PlayingSceneFB;

		EditorCamera* m_EditorCamera;

		GameViewportPanel* m_GameViewportPanel;
		SceneViewportPanel* m_SceneViewportPanel;
		HierarchyPanel* m_HierarchyPanel;

		Ref<AssetHandler> m_AssetHandler;

		AssetDatabaseEditor* m_EditorDB;

		Logger* m_Logger;

		bool m_Playing = false;
		bool m_Paused = false;
	};
}