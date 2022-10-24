#pragma once

namespace DuskEngine
{	
	template <typename Type>
	class Handle
	{
	public:
		Handle() = default;
		Handle(uint32_t value):m_Value(value) {}

		operator uint32_t() const { return m_Value; }

		friend bool operator==(Handle a, Handle b) { return a.m_Value == b.m_Value; }
		friend bool operator!=(Handle a, Handle b) { return a.m_Value != b.m_Value; }
	private:
		uint32_t m_Value;

		friend class AssetHandler;
	};
}