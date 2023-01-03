#pragma once
#include "Core/Application/Layer.h"

#include "Core/Events/WindowEvent.h"
#include "Core/ECS/Scene.h"
#include "Utils/Memory/Memory.h"

namespace DuskEngine
{
	class Logger;

	class RuntimeLayer : public Layer
	{
	public:
		RuntimeLayer();
		~RuntimeLayer();
		void OnUpdate() override;
		void OnEvent(Event& event) override;
	private:
		bool CameraProj(WindowResizeEvent& event);
		Ref<Scene> m_Scene;

		Logger* m_Logger;
	};
}