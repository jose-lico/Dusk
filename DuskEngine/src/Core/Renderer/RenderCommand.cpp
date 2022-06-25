#include "pch.h"
#include "RenderCommand.h"

#include "RendererAPI.h"

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

	void RenderCommand::UnbindVAO()
	{
		s_RendererAPI->UnbindVAO();
	}

	void RenderCommand::DrawIndexed(const UniqueRef<VertexArray>& vertexArray)
	{
		s_RendererAPI->DrawIndexed(vertexArray);
	}

	void RenderCommand::DrawArrays(unsigned int start, unsigned int count)
	{
		s_RendererAPI->DrawArrays(start, count);
	}
}