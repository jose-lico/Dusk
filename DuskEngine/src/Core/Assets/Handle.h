#pragma once

namespace DuskEngine
{
	template <typename Type, typename ValueType>
	class Handle
	{
	public:
		Handle() = default;
		Handle(ValueType value):m_Value(value) {}

		operator ValueType() const { return m_Value; }

		friend bool operator==(Handle a, Handle b) { return a.m_Value == b.m_Value; }
		friend bool operator!=(Handle a, Handle b) { return a.m_Value != b.m_Value; }
	private:
		ValueType m_Value;
	};
}