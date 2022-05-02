#pragma once
#include "PanelBase.h"

#include "Utils/Memory/Memory.h"

#include <functional>

namespace DuskEngine
{
	class EditorLayer;
	class Texture;

	class Toolbar : public Panel
	{
	public:
		Toolbar(bool* playing, const std::function<void(void)>& playfunc, const std::function<void(void)>& stopfunc, const std::function<void(void)>& pausefunc);
		~Toolbar() = default;

		virtual void OnImGuiRender() override;
	private:
		std::function<void(void)> m_PlayFunc;
		std::function<void(void)> m_StopFunc;
		std::function<void(void)> m_PauseFunc;

		bool* m_Playing;

		Ref<Texture> m_Play, m_Pause, m_Stop;
	};
}