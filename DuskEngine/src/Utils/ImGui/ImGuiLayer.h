#pragma once
#include "Core/Application/Layer.h"

namespace DuskEngine
{
	class Window;

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer(Window* window);
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnImGuiRender() override;
		void OnEvent(Event& event) override;

		void Begin();
		void End();
	private:
		void ApplyStyle();
		bool m_ShowDemoWindow = false;
		bool ActivateDemoWindow(KeyPressedEvent& e);

		// Owned by WindowManager
		Window* m_Window;
	};
}