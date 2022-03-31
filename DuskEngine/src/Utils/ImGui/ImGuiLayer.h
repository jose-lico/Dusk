#pragma once
#include "Core/Application/Layer.h"

namespace DuskEngine
{
	class Window;
	class KeyPressedEvent;

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
		void AddFont(const std::string& name, float size);
		void ApplyStyle();
		bool m_ShowDemoWindow = false;
		bool ActivateDemoWindow(KeyPressedEvent& e);

		Window* m_Window;
	};
}