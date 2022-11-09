#pragma once

#include "Core/Application/Core.h"

#include <iostream>

namespace DuskEngine
{
	struct OpenGLResource
	{
		uint32_t ResourceID;

		OpenGLResource() = default;

		~OpenGLResource()
		{
			std::cout << "Resource was destroyed" << std::endl; // Editor Logger is dead by the end of the application
		}

		OpenGLResource(const OpenGLResource& r)
		{
			ResourceID = r.ResourceID;
			LOG("Resource copy constructed");
		}

		OpenGLResource& operator=(const OpenGLResource& r)
		{
			ResourceID = r.ResourceID;
			LOG("Resource copy assigment constructed");
			return *this;
		}

		OpenGLResource(OpenGLResource&& r) noexcept
			: ResourceID{ r.ResourceID }
		{
			LOG("Resource move constructed");
		}

		OpenGLResource& operator=(OpenGLResource&& r) noexcept
		{
			ResourceID = r.ResourceID;
			LOG("Resource move assigment constructed");
			return *this;
		}
	};
}