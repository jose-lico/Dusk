#pragma once
#include "PanelBase.h"

#include "Core/ECS/Components.h"
#include "Platform/OpenGL/Texture.h"

namespace DuskEngine
{
	struct Texture;
	class Entity;
	class AssetHandler;
	class AssetDatabase;
	class AssetDatabaseEditor;

	enum class InspectionType
	{
		Entity,
		Image,
		None
	};

	class InspectorPanel : public Panel
	{
	public:
		InspectorPanel(AssetHandler* assetHandler, AssetDatabaseEditor* db);
		~InspectorPanel() = default;

		virtual void OnImGuiRender() override;
		inline void SetSelectedEntities(std::vector<Entity>& entities) { m_SelectedEntities = &entities; }
		inline void SetSelectableStatus(std::vector<bool>& status) { m_SelectableStatus = &status; }
		inline void SetCurrentImage(const Texture& texture) { m_CurrentImage = texture; }
		void SetInspectionType(const InspectionType& type);
	private:
		void MetaInspector();
		void TransformInspector();
		void CameraInspector();
		void LightInspector();
		void MeshRendererInspector();
		void ScriptInspector();

		std::vector<Entity>* m_SelectedEntities = nullptr;
		std::vector<bool>* m_SelectableStatus = nullptr;

		std::vector<std::string*> m_ModelList;
		std::vector<std::string*> m_MaterialList;
		std::vector<std::string*> m_ShaderList;

		Texture m_CurrentImage;
		AssetHandler* m_AssetHandler;
		AssetDatabase* m_DB;
		AssetDatabaseEditor* m_EditorDB;

		InspectionType m_CurrentInspection = InspectionType::None;
	};
}