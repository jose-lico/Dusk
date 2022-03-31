#pragma once
#include "Core/Macros/DUSK_API.h"

#include "Utils/Memory/Memory.h"

namespace DuskEngine
{
	class Mesh;

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