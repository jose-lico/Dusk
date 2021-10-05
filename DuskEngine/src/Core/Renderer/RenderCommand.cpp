#include "pch.h"
#include "RenderCommand.h"

namespace DuskEngine {

	RendererAPI* RenderCommand::s_RendererAPI;

	void RenderCommand::Init()
	{
		s_RendererAPI = RendererAPI::Create();
		s_RendererAPI->Init();
	}

	void RenderCommand::Shutdown()
	{
		//s_RendererAPI->Shutdown();
	}

	void RenderCommand::SetClearColor(const glm::vec4& color)
	{
		s_RendererAPI->SetClearColor(color);
	}

	void RenderCommand::Clear()
	{
		s_RendererAPI->Clear();
	}

	void RenderCommand::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
	{
		s_RendererAPI->DrawIndexed(vertexArray);
	}
}