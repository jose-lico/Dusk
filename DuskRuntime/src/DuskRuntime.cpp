#include "Core/Application/Application.h"
#include "EntryPoint.h"

#include "RuntimeLayer.h"

namespace DuskEngine
{
	class DuskRuntime : public Application
	{
	public:
		DuskRuntime(const ApplicationSpecs& specs)
			: Application(specs)
		{
			PushLayer(new RuntimeLayer());
		}

		~DuskRuntime()
		{
		}
	};

	Application* CreateApplication()
	{
		ApplicationSpecs spec;

		return new DuskRuntime(spec);
	}
}