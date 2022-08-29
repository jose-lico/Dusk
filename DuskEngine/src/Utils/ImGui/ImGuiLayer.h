#pragma once
#include "Core/Application/Layer.h"

namespace DuskEngine
{
	class Window;
	class KeyPressedEvent;
	struct Style;

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
		inline static Style* GetStyle() { return m_Style; }
	private:
		void AddFont(const std::string& name, float size);
		void ApplyStyle();
		bool m_ShowDemoWindow = false;
		bool ActivateDemoWindow(KeyPressedEvent& e);

		static Style* m_Style;
		Window* m_Window;
	};
}