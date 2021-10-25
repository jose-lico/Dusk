#pragma once

#include "Core/Renderer/Resources/VertexArray.h"
#include "Core/Renderer/Resources/Shader.h"
#include "Core/Renderer/Resources/Texture.h"

namespace DuskEngine
{
	struct MeshRenderer
	{
		// TODO: For now store a VertexArray, in the future swap for mesh object. Shader and Texture will be also part of a material object
		
		std::shared_ptr<VertexArray> VA;
		std::shared_ptr<Shader> SH;
		std::shared_ptr<Texture> TX;

		MeshRenderer() = default;
		MeshRenderer(std::shared_ptr<VertexArray>& va, std::shared_ptr<Shader>& shader, std::shared_ptr<Texture>& texture)
			:VA(va), SH(shader), TX(texture)
		{}
	};
}