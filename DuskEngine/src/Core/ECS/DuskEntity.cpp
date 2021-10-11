#include "pch.h"
#include "DuskEntity.h"

DuskEngine::DuskEntity::DuskEntity(entt::entity handle, Scene* scene)
	: m_EntityHandle(handle), m_Scene(scene)
{
}

void DuskEngine::DuskEntity::DestroyEntity()
{
	m_Scene->m_Registry.destroy(m_EntityHandle);
}
