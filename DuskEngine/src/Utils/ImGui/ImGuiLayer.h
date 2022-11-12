#pragma once
#include "Core/Application/Layer.h"

namespace DuskEngine
{
	class Window;
	class KeyPressedEvent;
	struct DuskImGuiStyle;

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer(Window* window);
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;

		void Begin();
		void End();

		void SetGLContext(Window* window);
		void AddFontFromFile(const std::string& path, float size);
	private:
		void AddFontFromMemory(void* fontData, float size);
		void ApplyStyle();
		bool ActivateDemoWindow(KeyPressedEvent& e);

		Window* m_Window;
		DuskImGuiStyle* m_Style;

		bool m_ShowDemoWindow = false;
	};
}