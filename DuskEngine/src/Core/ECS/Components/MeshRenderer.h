#pragma once

#include "Core/Renderer/Resources/VertexArray.h"
#include "Core/Renderer/Resources/Shader.h"
#include "Core/Renderer/Resources/Texture.h"
#include "Core/Renderer/Resources/Mesh.h"
#include "Core/Renderer/Resources/Material.h"

namespace DuskEngine
{
	struct MeshRenderer
	{
		std::shared_ptr<Mesh> MS;
		std::shared_ptr<Material> Mat;

		MeshRenderer() = default;
		MeshRenderer(std::shared_ptr<Mesh>& mesh, std::shared_ptr<Material>& material)
			:MS(mesh), Mat(material)
		{}
	};
}