#include "pch.h"
#include "Mesh.h"

#include "Platform/OpenGL/VertexArray.h"
#include "Platform/OpenGL/Buffer.h"

#include "Platform/OpenGL/OpenGLAPI.h"

namespace DuskEngine
{
	Mesh::Mesh(float* vertices, unsigned int size, unsigned int* indices, unsigned int count, MeshType type)
		:m_Type(type)
	{
		OpenGLAPI::GenVertexArrays(m_VA);
		OpenGLAPI::BindVertexArray(m_VA);

		VertexBufferLayout vbl;
		vbl.Push(ShaderDataType::FLOAT, 3, true);
		vbl.Push(ShaderDataType::FLOAT, 2, true);
		vbl.Push(ShaderDataType::FLOAT, 3, true);
		
		VertexBuffer vb;
		vb.BufferType = BufferType::ARRAY_BUFFER;
		vb.UsageType = UsageType::STATIC_DRAW;
		vb.Size = size;
		vb.Layout = vbl;

		OpenGLAPI::SetBufferData(vb, vertices);
		OpenGLAPI::SetVertexAttribPointer(vb);
		
		IndexBuffer ib;
		ib.BufferType = BufferType::ELEMENT_ARRAY_BUFFER;
		ib.UsageType = UsageType::STATIC_DRAW;
		ib.Size = count * sizeof(unsigned int);
		ib.Count = count;
		ib.UnitSize = sizeof(unsigned int);

		OpenGLAPI::SetBufferData(ib, indices);

		m_VA.VB = vb;
		m_VA.IB = ib;
	}

	Mesh::Mesh(std::vector<Vertex>& vertices, unsigned int* indices, unsigned int count)
		:m_Type(MeshType::Model)
	{
		OpenGLAPI::GenVertexArrays(m_VA);
		OpenGLAPI::BindVertexArray(m_VA);

		VertexBufferLayout vbl;
		vbl.Push(ShaderDataType::FLOAT, 3, true);
		vbl.Push(ShaderDataType::FLOAT, 2, true);
		vbl.Push(ShaderDataType::FLOAT, 3, true);

		VertexBuffer vb;
		vb.BufferType = BufferType::ARRAY_BUFFER;
		vb.UsageType = UsageType::STATIC_DRAW;
		vb.Size = vertices.size() * sizeof(Vertex);
		vb.Layout = vbl;

		OpenGLAPI::SetBufferData(vb, &vertices[0]);
		OpenGLAPI::SetVertexAttribPointer(vb);

		IndexBuffer ib;
		ib.BufferType = BufferType::ELEMENT_ARRAY_BUFFER;
		ib.UsageType = UsageType::STATIC_DRAW;
		ib.Size = count * sizeof(unsigned int);
		ib.Count = count;
		ib.UnitSize = sizeof(unsigned int);

		OpenGLAPI::SetBufferData(ib, indices);

		m_VA.VB = vb;
		m_VA.IB = ib;
	}

	Mesh::Mesh(std::vector<Vertex>& vertices, unsigned short* indices, unsigned int count)
		:m_Type(MeshType::Model)
	{
		OpenGLAPI::GenVertexArrays(m_VA);
		OpenGLAPI::BindVertexArray(m_VA);

		VertexBufferLayout vbl;
		vbl.Push(ShaderDataType::FLOAT, 3, true);
		vbl.Push(ShaderDataType::FLOAT, 2, true);
		vbl.Push(ShaderDataType::FLOAT, 3, true);

		VertexBuffer vb;
		vb.BufferType = BufferType::ARRAY_BUFFER;
		vb.UsageType = UsageType::STATIC_DRAW;
		vb.Size = vertices.size() * sizeof(Vertex);
		vb.Layout = vbl;

		OpenGLAPI::SetBufferData(vb, &vertices[0]);
		OpenGLAPI::SetVertexAttribPointer(vb);

		IndexBuffer ib;
		ib.BufferType = BufferType::ELEMENT_ARRAY_BUFFER;
		ib.UsageType = UsageType::STATIC_DRAW;
		ib.Size = count * sizeof(unsigned short);
		ib.Count = count;
		ib.UnitSize = sizeof(unsigned short);

		OpenGLAPI::SetBufferData(ib, indices);

		m_VA.VB = vb;
		m_VA.IB = ib;
	}

	Mesh::Mesh(std::vector<Vertex>& vertices, void* indices, size_t indicesSize, size_t indexTypeSize)
		:m_Type(MeshType::Model)
	{
		OpenGLAPI::GenVertexArrays(m_VA);
		OpenGLAPI::BindVertexArray(m_VA);

		VertexBufferLayout vbl;
		vbl.Push(ShaderDataType::FLOAT, 3, true);
		vbl.Push(ShaderDataType::FLOAT, 2, true);
		vbl.Push(ShaderDataType::FLOAT, 3, true);

		VertexBuffer vb;
		vb.BufferType = BufferType::ARRAY_BUFFER;
		vb.UsageType = UsageType::STATIC_DRAW;
		vb.Size = vertices.size() * sizeof(Vertex);
		vb.Layout = vbl;

		OpenGLAPI::SetBufferData(vb, &vertices[0]);
		OpenGLAPI::SetVertexAttribPointer(vb);

		IndexBuffer ib;
		ib.BufferType = BufferType::ELEMENT_ARRAY_BUFFER;
		ib.UsageType = UsageType::STATIC_DRAW;
		ib.Size = indicesSize;
		ib.Count = indicesSize/indexTypeSize;
		ib.UnitSize = indexTypeSize;

		OpenGLAPI::SetBufferData(ib, indices);

		m_VA.VB = vb;
		m_VA.IB = ib;
	}
}