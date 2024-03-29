#pragma once
#include "PanelBase.h"

#include "Platform/OpenGL/Texture.h"

#include <functional>

namespace DuskEngine
{
	class EditorLayer;

	class Toolbar : public Panel
	{
	public:
		Toolbar(bool* playing, bool* paused, const std::function<void(void)>& playfunc, const std::function<void(void)>& stopfunc, const std::function<void(void)>& pausefunc);
		~Toolbar();

		virtual void OnImGuiRender() override;
	private:
		std::function<void(void)> m_PlayFunc;
		std::function<void(void)> m_StopFunc;
		std::function<void(void)> m_PauseFunc;

		bool* m_Playing;
		bool* m_Paused;

		Texture m_Play, m_Pause, m_Stop, m_StepForward, m_Reload;

		float m_AlphasButtons[4] = { 0.87f, 0.4f, 0.4f, 0.4f };
		float m_AlphasMenus[4] = { 0.87f, 0.87f, 0.87f, 0.87f};
		bool m_MenusOpen[4] = {false, false, false, false};
	};
}