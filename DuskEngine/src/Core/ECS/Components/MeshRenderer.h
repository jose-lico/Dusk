#pragma once

#include "Core/Renderer/Resources/Mesh.h"
#include "Core/Renderer/Resources/Material.h"

#include "visit_struct/visit_struct.hpp"

namespace DuskEngine
{
	struct MeshRenderer
	{
		std::shared_ptr<Mesh> mesh;
		std::shared_ptr<Material> material;

		MeshRenderer() = default;
		MeshRenderer(std::shared_ptr<Mesh>& mesh, std::shared_ptr<Material>& material)
			:mesh(mesh), material(material)
		{}
	};
}

VISITABLE_STRUCT(DuskEngine::MeshRenderer, mesh, material);