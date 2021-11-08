#pragma once

namespace DuskEngine
{
	class Panel
	{
	public:
		Panel() = default;
		virtual ~Panel() = default;

		virtual void OnImGuiRender() = 0;
	};
}
