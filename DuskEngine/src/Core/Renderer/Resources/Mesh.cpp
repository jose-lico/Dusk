#include "pch.h"
#include "Mesh.h"

#include "Core/Macros/LOG.h"

namespace DuskEngine
{
	Mesh::Mesh(float* vertices, unsigned int size, unsigned int* indices, unsigned int count)
	{
		m_VA.reset(VertexArray::Create());
		m_VA->Bind();

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, size));

		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, count));

		std::shared_ptr<VertexBufferLayout> vbl;
		vbl = std::make_shared<VertexBufferLayout>();
		vbl->Push(ShaderDataType::Float, 3, true);
		vbl->Push(ShaderDataType::Float, 2, true);
		vbl->Push(ShaderDataType::Float, 3, true);
		vertexBuffer->SetLayout(vbl);

		m_VA->AddBuffer(vertexBuffer);
		m_VA->AddIndices(indexBuffer);
	}

	Mesh::~Mesh()
	{
	}
}