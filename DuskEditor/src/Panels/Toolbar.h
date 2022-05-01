#pragma once
#include "PanelBase.h"

namespace DuskEngine
{

	class Toolbar : public Panel
	{
	public:
		Toolbar(bool* playing);
		~Toolbar() = default;

		virtual void OnImGuiRender() override;
	private:
		bool* m_Playing;
	};
}