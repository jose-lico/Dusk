#pragma once

#include "Core/Application/Layer.h"

namespace DuskEngine
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer() = default;
		~ImGuiLayer() = default;

		void OnAttach() override;
		void OnDetach() override;
		void OnImGuiRender() override;
		void OnEvent(Event& event) override;

		void Begin();
		void End();
	private:
		bool m_ShowDemoWindow = true;
		bool ActivateDemoWindow(KeyPressedEvent& e);
	};
}