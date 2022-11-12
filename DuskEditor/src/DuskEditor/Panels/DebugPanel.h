#pragma once
#include "PanelBase.h"

namespace DuskEngine
{
	struct TimerNode;

	class DebugPanel : public Panel
	{
	public:
		DebugPanel();
		~DebugPanel();

		virtual void OnImGuiRender() override;
	private:
		TimerNode* m_StartupRoot;
		float m_RefreshFPSTimer = 0.0f;
		float m_Frametime = 0.0f;
	};
}