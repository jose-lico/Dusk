#include "pch.h"
#include "Renderer.h"

#include "RenderCommand.h"

#include "Core/Macros/LOG.h"
#include "Core/Assets/Assets/Mesh.h"
#include "Resources/VertexArray.h"

namespace DuskEngine {

	void Renderer::BeginScene()
	{
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::Submit(const Ref<Mesh>& mesh)
	{
		mesh->m_VA->Bind();
		RenderCommand::DrawIndexed(mesh->m_VA);
	}
}