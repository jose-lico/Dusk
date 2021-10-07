#pragma once

#include "Core/Events/Event.h"
#include "Core/Application/Macros.h"

namespace DuskEngine
{
	class DUSK_API Layer
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