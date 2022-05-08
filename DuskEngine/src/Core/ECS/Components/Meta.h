#pragma once


#include <string>
#include <stdio.h>

namespace DuskEngine
{
	struct Meta
	{
		bool enabled;
		std::string name;

		// For scripting testing purposes
		Meta() 
			:enabled(true) 
		{ 
			printf("Default Meta constructor\n"); 
		}
		Meta(const std::string& name)
			:name(name), enabled(true) {}

		~Meta() 
		{ 
		//	printf("Bye bye\n"); 
		}
	};
}