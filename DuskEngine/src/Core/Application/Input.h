#pragma once

#include "Core/Events/KeyEvent.h"
#include "Core/Events/MouseEvent.h"

namespace DuskEngine
{
	class Input 
	{
	public:
		static bool IsKeyPressed(const KeyCode keycode);
		static bool IsMouseButtonPressed(const MouseCode mousecode);
		static float GetMouseX();
		static float GetMouseY();
	};
}