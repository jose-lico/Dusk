#include "pch.h"
#include "Renderer.h"

#include "Core/Macros/LOG.h"

#include "RenderCommand.h"

namespace DuskEngine {

	void Renderer::Init()
	{
		RenderCommand::Init();

		TRACE("Renderer Initilialized")
	}

	void Renderer::Shutdown()
	{
		TRACE("Renderer shutdown")
	}

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