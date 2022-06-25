#include "pch.h"
#include "Mesh.h"

#include "Platform/OpenGL/VertexArray.h"
#include "Platform/OpenGL/Buffer.h"

namespace DuskEngine
{
	Mesh::Mesh(float* vertices, unsigned int size, unsigned int* indices, unsigned int count, MeshType type)
		:m_Type(type)
	{
		m_VA = MakeUnique<VertexArray>();
		m_VA->Bind();

		UniqueRef<VertexBuffer> vb = MakeUnique<VertexBuffer>(vertices, size);
		UniqueRef<VertexBufferLayout> vbl = MakeUnique<VertexBufferLayout>();
		vbl->Push(ShaderDataType::Float, 3, true);
		vbl->Push(ShaderDataType::Float, 2, true);
		vbl->Push(ShaderDataType::Float, 3, true);
		vb->SetLayout(vbl);
	
		UniqueRef<IndexBuffer> ib = MakeUnique<IndexBuffer>(indices, count);

		m_VA->AddBuffer(vb);
		m_VA->AddIndices(ib);
	}

	Mesh::Mesh(std::vector<Vertex>& vertices, unsigned int* indices, unsigned int count)
		:m_Type(MeshType::Model)
	{
		m_VA = MakeUnique<VertexArray>();
		m_VA->Bind();

		UniqueRef<VertexBuffer> vb = MakeUnique<VertexBuffer>(&vertices[0], (unsigned int)vertices.size() * sizeof(Vertex));
		UniqueRef<VertexBufferLayout> vbl = MakeUnique<VertexBufferLayout>();
		vbl->Push(ShaderDataType::Float, 3, true);
		vbl->Push(ShaderDataType::Float, 2, true);
		vbl->Push(ShaderDataType::Float, 3, true);
		vb->SetLayout(vbl);

		UniqueRef<IndexBuffer> ib = MakeUnique<IndexBuffer>(indices, count);

		m_VA->AddBuffer(vb);
		m_VA->AddIndices(ib);
	}

	Mesh::~Mesh()
	{
	}
}