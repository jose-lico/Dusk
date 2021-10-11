#include "pch.h"
#include "Scene.h"

#include "DuskEntity.h"

namespace DuskEngine
{
	Scene::Scene()
	{
	}

	void Scene::OnUpdate()
	{
		
	}

	DuskEntity Scene::CreateEntity()
	{
		DuskEntity entity = { m_Registry.create(), this };
		return entity;
	}
}