#include "DuskEngine.h"

#include <iostream>

namespace DuskEngine
{
	DuskEngine::DuskEngine()
	{
	}

	int DuskEngine::Sum(int a, int b)
	{
		return a + b;
	}

	void DuskEngine::Greeting()
	{
		std::cout << "This is a message from the Dusk Engine DLL!" << std::endl;
	}
}