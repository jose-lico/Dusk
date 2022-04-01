#include "Core/Application/Application.h"
#include "EntryPoint.h"

#include "RuntimeLayer.h"

namespace DuskEngine
{
	class Game : public Application
	{
	public:
		Game()
		{
			PushLayer(new RuntimeLayer());
		}

		~Game()
		{
		}
	};

	Application* CreateApplication()
	{
		return new Game();
	}
}