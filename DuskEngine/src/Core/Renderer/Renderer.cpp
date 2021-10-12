#include "pch.h"
#include "Renderer.h"

#include "Core/Macros/LOG.h"

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

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}