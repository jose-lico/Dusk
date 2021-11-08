#pragma once

namespace DuskEngine
{
	class Panel
	{
	public:
		Panel() = default;
		~Panel() = default;

		virtual void OnImGuiRender() = 0;
	};
}
