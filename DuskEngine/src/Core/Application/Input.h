#pragma once

#include "Core/Events/KeyEvent.h"
#include "Core/Events/MouseEvent.h"
#include "Core/Application/Macros.h"

namespace DuskEngine
{
	class DUSK_API Input 
	{
	public:
		static bool IsKeyPressed(const KeyCode keycode);
		static bool IsMouseButtonPressed(const MouseCode mousecode);
		static float GetMouseX();
		static float GetMouseY();
		static void SetCursorActive(CursorState state);
	};
}