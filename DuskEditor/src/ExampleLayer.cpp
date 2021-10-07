//#include "ExampleLayer.h"
//
//#include "gtc/type_ptr.hpp"
//
//void ExampleLayer::OnAttach()
//{
//	m_Camera = new DuskEngine::Camera(glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.01f, 100.0f), glm::vec3(0.0f, 0.0f, 1.5f), glm::vec3(0.0f, -90.0f, 0.0f));
//
//	float vertices[] =
//	{
//		// positions        // texture coords
//		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // top right
//		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
//		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
//		-0.5f,  0.5f, 0.0f,	0.0f, 1.0f  // top left 
//	};
//
//	unsigned int indices[] =
//	{
//		0, 1, 3,   // first triangle
//		1, 2, 3    // second triangle
//	};
//
//	m_Shader = std::make_shared<DuskEngine::Shader>("res/shaders/simpleTexture.glsl");
//
//	m_Texture = std::make_shared<DuskEngine::Texture>("res/textures/uv_mapper.jpg", GL_RGB);
//
//	m_VA.reset(DuskEngine::VertexArray::Create());
//
//	std::shared_ptr<DuskEngine::VertexBuffer> vertexBuffer;
//	vertexBuffer.reset(DuskEngine::VertexBuffer::Create(vertices, sizeof(vertices)));
//
//	std::shared_ptr<DuskEngine::IndexBuffer> indexBuffer;
//	indexBuffer.reset(DuskEngine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));
//
//	DuskEngine::VertexBufferLayout vbl;
//	vbl.Push(DuskEngine::ShaderDataType::Float, 3, true);
//	vbl.Push(DuskEngine::ShaderDataType::Float, 2, true);
//	vertexBuffer->SetLayout(vbl);
//
//	m_VA->Bind();
//	m_VA->AddBuffer(vertexBuffer);
//	m_VA->AddIndices(indexBuffer);
//}
//
//void ExampleLayer::OnDetach()
//{
//}
//
//void ExampleLayer::OnUpdate()
//{
//	DuskEngine::RenderCommand::SetClearColor({ 1.0f, 1.0f, 0.0f, 1 });
//	DuskEngine::RenderCommand::Clear();
//
//	m_Shader->Bind();
//	m_Shader->SetUniformMat4("u_ModelViewProjection", m_Camera->GetViewProjectionMatrix());
//	m_Texture->Bind(0);
//	DuskEngine::Renderer::Submit(m_VA);
//}
//
//void ExampleLayer::OnImGuiRender()
//{
//}
//
//void ExampleLayer::OnEvent(DuskEngine::Event& event)
//{
//}
