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
		//static TimerNode* m_RootTimer;
	};
}