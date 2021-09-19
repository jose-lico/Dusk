#pragma once

#include "Core/Window.h"
#include "GLFW/glfw3.h"
#include "Core/Renderer/RendererContext.h"

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

		void SetVSync(bool vsync) override;
		bool IsVSync() const override;

		bool ShouldClose() const override;

		void* GetNativeHandle() const override;
	private:
		void Init(const WindowData& data);
		void Shutdown();

		GLFWwindow* m_Window;
		RendererContext* m_Context;
		WindowData m_Data;
	};
}