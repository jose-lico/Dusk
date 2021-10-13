#pragma once
#include "Core/Macros/DUSK_API.h"

#include "ComponentBase.h"

#include "Core/Renderer/VertexArray.h"
#include "Core/Renderer/Shader.h"
#include "Platform/OpenGL/Texture.h"

#include <glm/glm.hpp>

namespace DuskEngine
{
	class DUSK_API RendererComponent : public Component
	{
	public:
		RendererComponent(std::shared_ptr<VertexBuffer>& vb, std::shared_ptr<IndexBuffer>& ib,\
			std::shared_ptr<Shader>& shader, std::shared_ptr<Texture>& texture);
		~RendererComponent() = default;

		std::shared_ptr<VertexArray> VA;
		std::shared_ptr<Shader> SH;
		std::shared_ptr<Texture> TX;

		std::shared_ptr<glm::mat4> viewProjectionMatrix;
		std::shared_ptr<glm::mat4> modelMatrix;

		virtual void UpdateComponent() override;
	};
}
