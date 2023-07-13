#pragma once
#include "Core/Application/Layer.h"

#include <string>

namespace DuskEngine
{
	class Window;
	class KeyPressedEvent;
	struct DuskImGuiStyle;

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;

		void SetGLContext(Window* window);
		void DestroyGLContext();
		
		void Begin() const;
		void End() const;

		void AddFontFromFile(const std::string& path, float size) const;
	private:
		void AddFontFromMemory(void* fontData, float size) const;
		bool ActivateDemoWindow(KeyPressedEvent& e);
		void ApplyStyle() const;

		Window* m_Window = nullptr;
		DuskImGuiStyle* m_Style;

		bool m_ShowDemoWindow = false;
		std::string m_SettingsPath;
	};
}