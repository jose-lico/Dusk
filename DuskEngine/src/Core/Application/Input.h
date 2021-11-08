#pragma once
#include "Core/Macros/DUSK_API.h"

#include "Core/Events/KeyEvent.h"
#include "Core/Events/MouseEvent.h"

namespace DuskEngine
{
	class DUSK_EXPORT Input
	{
	public:
		static bool IsKeyPressed(const KeyCode keycode);
		static bool IsMouseButtonPressed(const MouseCode mousecode);
		static float GetMouseX();
		static float GetMouseY();
		static void SetCursorActive(CursorState state);
	};
}