#pragma once

#include "Core/Assets/Assets/Mesh.h"
#include "Core/Assets/Assets/Material.h"
#include "uuid.h"

namespace DuskEngine
{
	struct MeshRenderer
	{
		uuids::uuid mesh;
		Ref<Material> material;

		MeshRenderer() = default;
		/*MeshRenderer(Ref<Mesh>& mesh, Ref<Material>& material)
			:mesh(mesh), material(material)
		{}*/
	};
}