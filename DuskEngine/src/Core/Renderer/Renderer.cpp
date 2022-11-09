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
		m_Context = new OpenGLContext(window.GetNativeHandle());
	}

	Renderer::~Renderer()
	{
		delete m_Context;
	}

	void Renderer::BeginScene()
	{
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const VertexArray& vertexArray)
	{
		OpenGLAPI::BindVertexArray(vertexArray);
		OpenGLAPI::DrawIndexed(vertexArray);
	}

	void Renderer::Submit(const Mesh& mesh)
	{
		OpenGLAPI::BindVertexArray(mesh.GetVertexArray());
		OpenGLAPI::DrawIndexed(mesh.GetVertexArray());
	}

	void Renderer::SwapBuffers()
	{
		m_Context->SwapBuffers();
	}
}