#pragma once

namespace DuskEngine
{
	class IndexBuffer
	{
	private:
		unsigned int m_ID;
		unsigned int m_Count;
	public:
		IndexBuffer(unsigned int count, const void* data);
		~IndexBuffer();

		void Bind() const;
		void Unbind() const;

		unsigned int GetCount() const { return m_Count; }
	};
}