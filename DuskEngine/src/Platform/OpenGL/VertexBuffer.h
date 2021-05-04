#pragma once

namespace DuskEngine
{
	class VertexBuffer
	{
	public:
		VertexBuffer(unsigned int size, const void* data);
		~VertexBuffer();

		void Bind() const;
		void Unbind() const;
	private:
		unsigned int m_ID;
	};
}

