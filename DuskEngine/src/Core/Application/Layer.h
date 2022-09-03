#pragma once

namespace DuskEngine
{
	class Event;

	class Layer
	{
	public:
		Layer():Enabled(1) {};
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		bool Enabled;
	};
}