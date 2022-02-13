#include "DuskEngine.h"
#include "EntryPoint.h"

#include "GameLayer.h"

namespace DuskEngine
{
	class Game : public Application
	{
	public:
		Game()
		{
			PushLayer(new GameLayer());
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