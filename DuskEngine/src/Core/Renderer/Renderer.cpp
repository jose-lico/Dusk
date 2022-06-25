#include "pch.h"
#include "Renderer.h"

#include "RenderCommand.h"

#include "Core/Assets/Assets/Mesh.h"
#include "Platform/OpenGL/VertexArray.h"

namespace DuskEngine {

	void Renderer::BeginScene()
	{
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const UniqueRef<VertexArray>& vertexArray)
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