#pragma once

namespace DuskEngine
{
	class Event;

	class Panel
	{
	public:
		Panel() = default;
		virtual ~Panel() = default;

		virtual void OnImGuiRender() = 0;
		virtual void OnEvent(Event& event) {}
	protected:
		void CheckFocus();
	};
}
