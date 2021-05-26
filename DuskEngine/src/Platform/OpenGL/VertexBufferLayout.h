#pragma once

#include "pch.h"
#include "GLCommon.h"

namespace DuskEngine 
{
	struct  VertexBufferElement
	{
		unsigned int type;
		unsigned int count;
		bool normalized;

		static unsigned int GetSizeOfType(unsigned int type)
		{
			switch (type)
			{
			case GL_FLOAT:			return 4;
			case GL_UNSIGNED_INT:	return 4;
			case GL_UNSIGNED_BYTE:	return 1;
			}

			return 0;
		}
	};

	class VertexBufferLayout
	{
	public:
		VertexBufferLayout()
			: m_Stride(0) {}

		template<typename T> 
		void Push(unsigned int count);

		const std::vector<DuskEngine::VertexBufferElement> GetElements() const { return m_Elements; }
		unsigned int GetStride() const { return m_Stride; }
	private:
		std::vector<DuskEngine::VertexBufferElement> m_Elements;
		int m_Stride;
	};
}