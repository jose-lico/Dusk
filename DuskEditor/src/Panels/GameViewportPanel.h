#pragma once
#include "PanelBase.h"

#include "Core/ECS/Entity.h"

#include "Utils/Memory/Memory.h"

#include "glm/glm.hpp"

#include <vector>

namespace DuskEngine
{
	class Framebuffer;

	class GameViewportPanel : public Panel
	{
	public:
		GameViewportPanel(Ref<Framebuffer>& fb, Entity camera, bool* playing);
		~GameViewportPanel() = default;

		virtual void OnImGuiRender() override;
		void SetCamera(Entity* targetCamera) { m_Camera = *targetCamera; }
	private:
		WeakRef<Framebuffer> m_FB;
		glm::vec2 m_ViewportSize;
		Entity m_Camera;
		bool* m_Playing;
	};
}