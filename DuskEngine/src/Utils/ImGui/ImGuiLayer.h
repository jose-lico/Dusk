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

		void AddFontFromFile(const std::string& path, float size);
	private:
		void AddFontFromMemory(void* fontData, float size);
		void ApplyStyle();
		bool ActivateDemoWindow(KeyPressedEvent& e);

		Window* m_Window;

		bool m_ShowDemoWindow = false;
	};
}