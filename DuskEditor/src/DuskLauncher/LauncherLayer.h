#pragma once
#include "Core/Application/Layer.h"

#include "Dockspace.h"

namespace DuskEngine
{
	class Logger;
	class DuskEditor;

	class LauncherLayer : public Layer
	{
	public:
		LauncherLayer(DuskEditor* editor);
		~LauncherLayer();

		void OnUpdate() override;
		void OnImGuiRender() override;
	private:
		Dockspace m_Dockspace;

		DuskEditor* m_Editor;
		Logger* m_Logger;
		
		bool m_LaunchEditor = false;
	};
}