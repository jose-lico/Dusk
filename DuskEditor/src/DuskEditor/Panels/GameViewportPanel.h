#pragma once
#include "PanelBase.h"

#include "Core/ECS/Entity.h"

#include "glm/glm.hpp"

#include <vector>

namespace DuskEngine
{
	struct Framebuffer;

	class GameViewportPanel : public Panel
	{
	public:
		GameViewportPanel(Framebuffer* fb, Entity camera, bool* playing);
		~GameViewportPanel() = default;

		virtual void OnImGuiRender() override;
		void SetCamera(Entity* targetCamera) { m_Camera = *targetCamera; }
	private:
		Framebuffer* m_FB;
		glm::vec2 m_ViewportSize;
		Entity m_Camera;
		bool* m_Playing;
	};
}