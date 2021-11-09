#pragma once
#include "Core/Macros/DUSK_API.h"

#include "Utils/Memory/Memory.h"

#include "Core/Renderer/Resources/Mesh.h"

namespace DuskEngine
{
	class DUSK_EXPORT PrimitiveMesh
	{
	public:
		static Ref<Mesh> Quad();
		static Ref<Mesh> Cube();
	private:
		static const char* m_Primitives[];
		friend class InspectorPanel;
	};
}