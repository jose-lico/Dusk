#include "pch.h"
#include "Renderer.h"

#include "Utils/Logging/Log.h"

namespace DuskEngine {

	void Renderer::Init()
	{
		RenderCommand::Init();

		DUSK_LOG_INFO("Renderer intialized")
	}

	void Renderer::Shutdown()
	{
		DUSK_LOG_INFO("Renderer shutdown")
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