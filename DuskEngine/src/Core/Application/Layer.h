#pragma once

namespace DuskEngine
{
	class Layer
	{
	public:
		Layer():Enabled(1) {};
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnImGuiRender() {}
		bool Enabled;
	};
}