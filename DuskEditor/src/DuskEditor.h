#pragma once
#include "Core/Application/Application.h"

namespace DuskEngine
{
	struct ApplicationSpecs;
	struct CliOptions;

	class Layer;

	class DuskEditor : public Application
	{
	public:
		DuskEditor(const ApplicationSpecs& specs, const CliOptions& options);
		~DuskEditor();

		void LaunchEditor();
		std::string ProjectPath;
	private:
		Layer* m_Layer = nullptr;
	};
}