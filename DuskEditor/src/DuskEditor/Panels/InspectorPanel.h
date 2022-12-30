#pragma once
#include "PanelBase.h"

#include "Core/ECS/Components.h"

namespace DuskEngine
{
	class Entity;
	class AssetHandler;
	class AssetDatabase;
	class AssetDatabaseEditor;

	class InspectorPanel : public Panel
	{
	public:
		InspectorPanel(AssetHandler* assetHandler, AssetDatabaseEditor* db);
		~InspectorPanel() = default;

		virtual void OnImGuiRender() override;
		inline void SelectedEntities(std::vector<Entity>& entities) { m_SelectedEntities = &entities; }
	private:
		void MetaInspector();
		void TransformInspector();
		void CameraInspector();
		void LightInspector();
		void MeshRendererInspector();
		void ScriptInspector();

		std::vector<Entity>* m_SelectedEntities = nullptr;

		std::vector<std::string*> m_ModelList;
		std::vector<std::string*> m_MaterialList;
		std::vector<std::string*> m_ShaderList;

		AssetHandler* m_AssetHandler;
		AssetDatabase* m_DB;
		AssetDatabaseEditor* m_EditorDB;

		static void ScriptInspector(std::vector<Script*>& scripts);
	};
}