#pragma once

#include "Core/Application/Layer.h"

namespace DuskEngine
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer() = default;
		~ImGuiLayer() = default;

		void OnAttach() override;
		void OnDetach() override;

		void Begin();
		void End();
	};
}