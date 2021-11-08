#pragma once
#include "Core/Macros/DUSK_API.h"

#include "Core/Events/Event.h"

namespace DuskEngine
{
	class DUSK_EXPORT Layer
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