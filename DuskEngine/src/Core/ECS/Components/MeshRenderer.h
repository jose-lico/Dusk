#pragma once

#include "Core/Assets/Handle.h"

#include "uuid.h"

namespace DuskEngine
{
	class Mesh;
	class Material;

	struct MeshRenderer
	{
		Handle<Mesh> meshHandle;
		Handle<Material> materialHandle;
	};
}