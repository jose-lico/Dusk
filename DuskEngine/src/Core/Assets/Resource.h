#pragma once

namespace DuskEngine
{
	class Resource
	{
	public:
		Resource() : ResourceID(0) {}
		virtual ~Resource() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void Free() = 0;
	protected:
		uint32_t ResourceID;
	};
}