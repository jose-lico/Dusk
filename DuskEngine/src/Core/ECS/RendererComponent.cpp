#include "pch.h"
#include "RendererComponent.h"

#include "Core/Renderer/Renderer.h"

namespace DuskEngine
{
	RendererComponent::RendererComponent(std::shared_ptr<VertexBuffer>& vb, std::shared_ptr<IndexBuffer>& ib,\
		std::shared_ptr<Shader>& shader, std::shared_ptr<Texture>& texture)
	{
		VA.reset(DuskEngine::VertexArray::Create());
		VA->Bind();
		VA->AddBuffer(vb);
		VA->AddIndices(ib);

		TX = texture;
		SH = shader;
	}

	void RendererComponent::UpdateComponent()
	{
		TX->Bind(0);
		SH->Bind();
		SH->SetUniformMat4("u_ViewProjection", *viewProjectionMatrix);
		SH->SetUniformMat4("u_Model", *modelMatrix);
		Renderer::Submit(VA);
	}
}

