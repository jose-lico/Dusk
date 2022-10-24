#pragma once

namespace DuskEngine
{
	class Mesh;

	class PrimitiveMesh
	{
	public:
		static UniqueRef<Mesh> Quad();
		static UniqueRef<Mesh> Cube();
	private:
		static const char* m_Primitives[];
		friend class InspectorPanel;
	};
}