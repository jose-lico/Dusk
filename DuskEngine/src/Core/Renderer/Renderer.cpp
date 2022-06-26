#include "pch.h"
#include "Renderer.h"

#include "Core/Assets/Assets/Mesh.h"
#include "Platform/OpenGL/VertexArray.h"
#include "Platform/OpenGL/OpenGLAPI.h"
#include "Platform/OpenGL/OpenGLContext.h"
#include "Core/Application/Window.h"

namespace DuskEngine {

	Renderer::Renderer(const Window& window)
	{
		m_Context = MakeUnique<OpenGLContext>(window.GetNativeHandle());
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::BeginScene()
	{
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const UniqueRef<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		OpenGLAPI::DrawIndexed(vertexArray);
	}

	void Renderer::Submit(const UniqueRef<Mesh>& mesh)
	{
		mesh->m_VA->Bind();
		OpenGLAPI::DrawIndexed(mesh->m_VA);;
	}

	void Renderer::SwapBuffers()
	{
		m_Context->SwapBuffers();
	}
}