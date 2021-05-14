#pragma once

#include "Core/Window.h"
#include "GLFW/glfw3.h"

namespace DuskEngine
{
	class WindowsWindow : public Window 
	{
	public:
		WindowsWindow(const WindowData& data);
		~WindowsWindow() override;

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
		void* GetNativeHandle() const override;
	private:
		void Init(const WindowData& data);
		void Shutdown();

		GLFWwindow* m_Window;
		WindowData m_Data;
	};
}