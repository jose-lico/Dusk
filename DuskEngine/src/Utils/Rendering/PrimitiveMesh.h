#pragma once
#include "Core/Macros/DUSK_API.h"

#include "Core/Renderer/Resources/Mesh.h"

namespace DuskEngine
{
	class DUSK_EXPORT PrimitiveMesh
	{
	public:
		static std::shared_ptr<Mesh> Quad();
		static std::shared_ptr<Mesh> Cube();
	private:
		static const char* m_Primitives[];
		friend class InspectorPanel;
	};
}