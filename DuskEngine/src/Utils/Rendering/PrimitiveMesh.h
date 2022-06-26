#pragma once
#include "Core/Application/Core.h"

#include "Utils/Memory/Memory.h"

namespace DuskEngine
{
	class Mesh;

	class DUSK_EXPORT PrimitiveMesh
	{
	public:
		static UniqueRef<Mesh> Quad();
		static UniqueRef<Mesh> Cube();
	private:
		static const char* m_Primitives[];
		friend class InspectorPanel;
	};
}