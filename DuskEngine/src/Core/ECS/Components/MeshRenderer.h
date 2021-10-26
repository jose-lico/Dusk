#pragma once

#include "Core/Renderer/Resources/VertexArray.h"
#include "Core/Renderer/Resources/Shader.h"
#include "Core/Renderer/Resources/Texture.h"
#include "Core/Renderer/Resources/Mesh.h"

namespace DuskEngine
{
	struct MeshRenderer
	{
		// TODO: Shader and Texture will be also part of a material object
		
		std::shared_ptr<Mesh> MS;
		std::shared_ptr<Shader> SH;
		std::shared_ptr<Texture> TX;

		MeshRenderer() = default;
		MeshRenderer(std::shared_ptr<Mesh>& mesh, std::shared_ptr<Shader>& shader, std::shared_ptr<Texture>& texture)
			:MS(mesh), SH(shader), TX(texture)
		{}
	};
}