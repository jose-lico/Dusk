#pragma once

#include "Components/Transform.h"
#include "Components/Camera.h"

namespace DuskEngine
{
	class EditorCamera
	{
	public:
		EditorCamera() = default;
		~EditorCamera() = default;

		Transform transform;
		Camera camera;
	};
}