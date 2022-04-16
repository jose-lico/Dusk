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

		void Move() {};

		Transform transform;
		Camera camera;
	};
}