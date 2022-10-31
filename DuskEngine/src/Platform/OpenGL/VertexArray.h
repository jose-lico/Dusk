#pragma once

#include "Buffer.h"

namespace DuskEngine
{
	struct VertexArray
	{
		uint32_t ResourceID;
		VertexBuffer VB;
		IndexBuffer IB;
	};
}