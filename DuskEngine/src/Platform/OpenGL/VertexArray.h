#pragma once

#include "Buffer.h"

namespace DuskEngine
{
	struct VertexArray : OpenGLResource
	{
		VertexBuffer VB;
		IndexBuffer IB;
	};
}