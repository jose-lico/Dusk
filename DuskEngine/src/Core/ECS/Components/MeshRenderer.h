#pragma once

#include "Core/Resources/Resources/Mesh.h"
#include "Core/Resources/Resources/Material.h"
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