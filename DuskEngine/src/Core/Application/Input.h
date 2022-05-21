#pragma once

#include "Core/Events/KeyEvent.h"
#include "Core/Events/MouseEvent.h"

namespace DuskEngine::Input
{
	bool IsKeyPressed(const KeyCode keycode);
	bool IsMouseButtonPressed(const MouseCode mousecode);
	float GetMouseX();
	float GetMouseY();
	void SetCursorActive(CursorState state);
}