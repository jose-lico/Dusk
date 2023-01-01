#include "Core/Application/Application.h"
#include "EntryPoint.h"

#include "RuntimeLayer.h"

namespace DuskEngine
{
	class DuskRuntime : public Application
	{
	public:
		DuskRuntime(const ApplicationSpecs& specs, const CliOptions& options)
			: Application(specs, options)
		{
			PushLayer(new RuntimeLayer());
		}

		~DuskRuntime()
		{
		}
	};

	Application* CreateApplication(int argc, char** argv)
	{
		ApplicationSpecs spec;
		spec.Name = "Dusk Runtime";

		CliOptions options;
		options.ExeName = argv[0];

		return new DuskRuntime(spec, options);
	}
}