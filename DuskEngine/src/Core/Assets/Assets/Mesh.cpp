#include "pch.h"
#include "Mesh.h"

#include "Platform/OpenGL/VertexArray.h"
#include "Platform/OpenGL/Buffer.h"

namespace DuskEngine
{
	Mesh::Mesh(float* vertices, unsigned int size, unsigned int* indices, unsigned int count, MeshType type)
		:m_Type(type)
	{
		VertexBufferLayout vbl;
		vbl.Push(ShaderDataType::Float, 3, true);
		vbl.Push(ShaderDataType::Float, 2, true);
		vbl.Push(ShaderDataType::Float, 3, true);
		
		m_VA.Bind();

		VertexBuffer vb;
		vb.SetData(vertices, size);
		vb.SetLayout(vbl);
		
		IndexBuffer ib;
		ib.SetData(indices, count);

		m_VA.SetBuffer(vb);
		m_VA.SetIndices(ib);
	}

	Mesh::Mesh(std::vector<Vertex>& vertices, unsigned int* indices, unsigned int count)
		:m_Type(MeshType::Model)
	{
		VertexBufferLayout vbl;
		vbl.Push(ShaderDataType::Float, 3, true);
		vbl.Push(ShaderDataType::Float, 2, true);
		vbl.Push(ShaderDataType::Float, 3, true);

		m_VA.Bind();

		VertexBuffer vb;
		vb.SetData(&vertices[0], (unsigned int)vertices.size() * sizeof(Vertex));
		vb.SetLayout(vbl);

		IndexBuffer ib;
		ib.SetData(indices, count);

		m_VA.SetBuffer(vb);
		m_VA.SetIndices(ib);
	}

	Mesh::~Mesh()
	{
	}
}