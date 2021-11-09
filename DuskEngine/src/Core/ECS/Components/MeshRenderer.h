#pragma once

#include "Core/Renderer/Resources/Mesh.h"
#include "Core/Renderer/Resources/Material.h"

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