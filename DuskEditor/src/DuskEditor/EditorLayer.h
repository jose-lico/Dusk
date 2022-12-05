#pragma once
#include "Core/Application/Layer.h"

#include "DuskEditor/Utils/EditorCamera.h"
#include "Platform/OpenGL/Framebuffer.h"
#include "Utils/Memory/Memory.h"

#include "Panels.h"

#include <vector>

namespace DuskEngine
{
	class Scene;
	class EditorCamera;
	class AssetDatabaseEditor;
	class Logger;

	class EditorLayer : public Layer
	{
	public:
		EditorLayer(const std::string& path);
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

		Framebuffer m_EditorSceneFB;
		Framebuffer m_PlayingSceneFB;

		EditorCamera m_EditorCamera;

		GameViewportPanel* m_GameViewportPanel;
		SceneViewportPanel* m_SceneViewportPanel;
		HierarchyPanel* m_HierarchyPanel;

		Ref<AssetHandler> m_AssetHandler;

		AssetDatabaseEditor* m_EditorDB;

		Logger* m_Logger;

		bool m_Playing = false;
		bool m_Paused = false;
		std::string m_ProjectPath;
	};
}