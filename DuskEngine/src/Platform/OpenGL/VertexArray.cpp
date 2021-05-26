#include "pch.h"

#include "VertexArray.h"

namespace DuskEngine
{
	VertexArray::VertexArray()
		:Indices(0)
	{
		glGenVertexArrays(1, &m_ID);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_ID);
	}

	void VertexArray::AddBuffer(const VertexBuffer& vb)
	{
		vb.Bind();
	}

	void VertexArray::AddLayout(const VertexBufferLayout& layout)
	{
		const auto& elements = layout.GetElements();
		size_t offset = 0;
		for (unsigned int i = 0; i < elements.size(); i++) {
			const auto& element = elements[i];
			glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset);
			offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
			glEnableVertexAttribArray(i);
		}
	}

	void VertexArray::AddIndices(const IndexBuffer& ib)
	{
		ib.Bind();
		Indices = ib.GetCount();
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(m_ID);
	}
}