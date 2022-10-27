#pragma once

namespace DuskEngine
{
	class Mesh;

	class PrimitiveMesh
	{
	public:
		static Mesh Quad();
		static Mesh Cube();
	private:
		static const char* m_Primitives[];
		friend class InspectorPanel;
	};
}