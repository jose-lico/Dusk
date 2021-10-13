#pragma once

namespace DuskEngine
{
	class Component
	{
	public:
		Component() = default;
		~Component() = default;

		virtual void UpdateComponent() = 0;
		virtual void ImGuiComponent() {}
	};
}