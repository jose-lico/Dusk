#pragma once

#include "Core/Resources/Resources/Mesh.h"
#include "Core/Resources/Resources/Material.h"

#include "rttr/registration.h"
#include "visit_struct/visit_struct.hpp"

namespace DuskEngine
{
	struct MeshRenderer
	{
		Ref<Mesh> mesh;
		Ref<Material> material;

		MeshRenderer() = default;
		MeshRenderer(Ref<Mesh>& mesh, Ref<Material>& material)
			:mesh(mesh), material(material)
		{}
	};
}

VISITABLE_STRUCT(DuskEngine::MeshRenderer, mesh, material);

//RTTR_REGISTRATION
//{
//	rttr::registration::class_<DuskEngine::MeshRenderer>("MeshRenderer").
//		property("mesh", &DuskEngine::MeshRenderer::mesh).
//		property("material", &DuskEngine::MeshRenderer::material);
//}