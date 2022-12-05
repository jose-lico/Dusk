#pragma once

#include "Core/ECS/Components/Transform.h"
#include "Core/ECS/Components/Camera.h"

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