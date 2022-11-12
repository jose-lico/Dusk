#pragma once

namespace DuskEngine
{
	class Dockspace
	{
	public:
		Dockspace() = default;
		~Dockspace() = default;

		void BeginDockspace();
		void EndDockspace();
	};
}