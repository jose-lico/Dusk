#include "ExampleLayer.h"

#include "glm/gtc/type_ptr.hpp"

ExampleLayer::ExampleLayer()
{
	m_Camera = new DuskEngine::Camera(glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.01f, 100.0f), glm::vec3(0.0f, 0.0f, 1.5f), glm::vec3(0.0f, -90.0f, 0.0f));
}

void ExampleLayer::OnAttach()
{

	float vertices[] =
	{
		// positions        // texture coords
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,	0.0f, 1.0f  // top left 
	};

	unsigned int indices[] =
	{
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	m_Shader.reset(DuskEngine::Shader::Create("res/shaders/simpleTexture.glsl"));

	m_Texture = std::make_shared<DuskEngine::Texture>("res/textures/uv_mapper.jpg", GL_RGB);

	m_VA.reset(DuskEngine::VertexArray::Create());

	std::shared_ptr<DuskEngine::VertexBuffer> vertexBuffer;
	vertexBuffer.reset(DuskEngine::VertexBuffer::Create(vertices, sizeof(vertices)));

	std::shared_ptr<DuskEngine::IndexBuffer> indexBuffer;
	indexBuffer.reset(DuskEngine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));

	DuskEngine::VertexBufferLayout vbl;
	vbl.Push(DuskEngine::ShaderDataType::Float, 3, true);
	vbl.Push(DuskEngine::ShaderDataType::Float, 2, true);
	vertexBuffer->SetLayout(vbl);

	m_VA->Bind();
	m_VA->AddBuffer(vertexBuffer);
	m_VA->AddIndices(indexBuffer);
}

void ExampleLayer::OnUpdate()
{
	DuskEngine::RenderCommand::SetClearColor({ 1.0f, 0.0f, 0.0f, 1 });
	DuskEngine::RenderCommand::Clear();

	// dodgy camera controller
	{
		glm::vec3 pos = m_Camera->GetPosition();
		glm::vec3 input(0.0f);

		if (DuskEngine::Input::IsKeyPressed(DuskEngine::Key::D))
			input.x += 3.0f * DuskEngine::Time::GetDeltaTime();
		else if (DuskEngine::Input::IsKeyPressed(DuskEngine::Key::A))
			input.x -= 3.0f * DuskEngine::Time::GetDeltaTime();

		if (DuskEngine::Input::IsKeyPressed(DuskEngine::Key::W))
			input.z -= 3.0f * DuskEngine::Time::GetDeltaTime();
		else if (DuskEngine::Input::IsKeyPressed(DuskEngine::Key::S))
			input.z += 3.0f * DuskEngine::Time::GetDeltaTime();

		if (DuskEngine::Input::IsKeyPressed(DuskEngine::Key::Q))
			input.y += 3.0f * DuskEngine::Time::GetDeltaTime();
		else if (DuskEngine::Input::IsKeyPressed(DuskEngine::Key::E))
			input.y -= 3.0f * DuskEngine::Time::GetDeltaTime();

		pos += input;
		m_Camera->SetPosition(pos);

		glm::vec3 rot = m_Camera->GetRotation();

		if (!movingCamera && DuskEngine::Input::IsMouseButtonPressed(DuskEngine::Mouse::MOUSE_BUTTON_2))
		{
			DuskEngine::Input::SetCursorActive(DuskEngine::Cursor::CURSOR_DISABLED);
			movingCamera = true;
			firstMouse = true;
		}
		else if (!DuskEngine::Input::IsMouseButtonPressed(DuskEngine::Mouse::MOUSE_BUTTON_2))
		{
			DuskEngine::Input::SetCursorActive(DuskEngine::Cursor::CURSOR_NORMAL);
			movingCamera = false;
		}

		if (movingCamera)
		{
			if (firstMouse)
			{
				lastX = DuskEngine::Input::GetMouseX();
				lastY = DuskEngine::Input::GetMouseY();
				firstMouse = false;
			}

			float xoffset = DuskEngine::Input::GetMouseX() - lastX;
			float yoffset = lastY - DuskEngine::Input::GetMouseY(); // reversed since y-coordinates go from bottom to top

			lastX = DuskEngine::Input::GetMouseX();
			lastY = DuskEngine::Input::GetMouseY();

			xoffset *= 20.0f * DuskEngine::Time::GetDeltaTime();
			yoffset *= 20.0f * DuskEngine::Time::GetDeltaTime();

			rot.x += yoffset;
			rot.y += xoffset;
			m_Camera->SetRotation(rot);
		}
	}

	m_Shader->Bind();

	m_Shader->SetUniformMat4("u_ModelViewProjection", m_Camera->GetViewProjectionMatrix());
	m_Texture->Bind(0);
	DuskEngine::Renderer::Submit(m_VA);
}