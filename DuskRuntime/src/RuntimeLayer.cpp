#include "RuntimeLayer.h"

// temp
#include "DuskEditor/Assets/AssetDatabaseEditor.h"

#include "Core/Application/Application.h"
#include "Core/Application/Window.h"
#include "Core/Application/Input.h"
#include "Core/Application/Time.h"
#include "Core/Serialization/SceneSerializer.h"
#include "Core/ECS/Components/Camera.h"
#include "Core/ECS/Entity.h"
#include "Core/Assets/AssetDatabase.h"
#include "Platform/OpenGL/OpenGLAPI.h"
#include "Utils/Profiling/Timer.h"

#include "GLFW/glfw3.h"
#include "glm/gtc/type_ptr.hpp"

#include <fstream>

namespace DuskEngine
{
	RuntimeLayer::RuntimeLayer()
	{
		m_Logger = new Logger(LOGGER);

		auto& app = Application::Get();

		WindowData data;
		const ApplicationSpecs& specs = app.GetSpecs();
		data.Title = specs.Name + " | " + specs.Platform + " | " + specs.Target + " | Runtime";
		Window& window = app.CreateWindow(data);
		OpenGLAPI::CreateContext(window.GetNativeHandle());
#if DUSK_IMGUI
		app.SetImGuiGLContext();
#endif	
		window.Maximize();

		// Very temp as runtime will have its own project loading stuff with vfs
		{
			//Timer databaseTimer("Loading project assets");
			auto& db = Application::Get().GetAssetDatabase();

			auto runtimeDB = new AssetDatabaseEditor(&db, std::filesystem::current_path().string());

			{
				ScopeTimer registerAssets("Register Assets");
				runtimeDB->RegisterAssets();
			}

			delete(runtimeDB);
		}

		// hardcoded project path, should be current dir
		m_Scene = MakeRef<Scene>("Runtime Scene", "");

		SceneSerializer::DeserializeText(m_Scene, "scenes/scene.yaml");

		m_Scene->OnAwakeRuntime();
	}

	RuntimeLayer::~RuntimeLayer()
	{
		m_Scene->OnShutdownRuntime();
	}

	void RuntimeLayer::OnUpdate()
	{
		m_Scene->OnUpdateRuntime(true);
	}

	void RuntimeLayer::OnImGuiRender()
	{
	}

	void RuntimeLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(RuntimeLayer::CameraProj));
	}

	bool RuntimeLayer::CameraProj(WindowResizeEvent& event)
	{
		auto camera = m_Scene->GetMainCamera();
		camera->GetComponent<Camera>().projectionMatrix =
			glm::perspective(glm::radians(45.0f), (float)(event.GetWidth()/event.GetHeight()), 0.01f, 100.0f);

		return true;
	}
	
	void DropCallback(GLFWwindow* window, int count, const char** paths)
	{
		
	}
}