#include "pch.h"
#include "SceneEntt.h"

namespace DuskEngine
{
	SceneEntt::SceneEntt()
	{
	}

	SceneEntt::~SceneEntt()
	{
	}

	entt::entity SceneEntt::CreateEntity()
	{
		return m_Registry.create();
	}
}

