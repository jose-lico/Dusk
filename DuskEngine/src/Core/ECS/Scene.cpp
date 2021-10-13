#include "pch.h"
#include "Scene.h"

namespace DuskEngine
{
	Scene::Scene()
	{

	}

	Scene::~Scene()
	{
	}

	void Scene::AddEntity(std::shared_ptr<DuskEntity> entity)
	{
		m_Entities.push_back(entity);
	}

	void Scene::OnEntityUpdate()
	{
		for (std::shared_ptr<DuskEntity> ent : m_Entities)
			ent->UpdateComponents();
	}

	void Scene::OnEntityImGui()
	{
		for (std::shared_ptr<DuskEntity> ent : m_Entities)
			ent->ImGuiRender();
	}
}


