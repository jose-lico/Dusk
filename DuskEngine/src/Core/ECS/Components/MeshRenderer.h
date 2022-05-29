#pragma once

#include "uuid.h"

namespace DuskEngine
{
	struct MeshRenderer
	{
		uuids::uuid meshHandle;
		uuids::uuid materialHandle;

		MeshRenderer() = default;
	};
}