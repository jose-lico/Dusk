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

#include "zstd.h"

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
				Timer registerAssets("Register Assets");
				runtimeDB->RegisterAssets();
			}

			runtimeDB->ImportAssets();

			db.LoadProject();
		}

		// hardcoded project path, should be current dir
		m_Scene = MakeRef<Scene>("Runtime Scene", "");

		SceneSerializer::DeserializeText(m_Scene, "scenes/scene.yaml");

		m_Scene->OnAwakeRuntime();


		// temp compress image

		// get duck image, 4k so raw uncompressed is 64 mb

		std::ifstream duck(".import/images/duck.png-2bed8e21-0471-4527-8171-5a968a7b80de.import");
		std::stringstream buffer;
		buffer << duck.rdbuf();
		
		void* p_dst = malloc(buffer.str().size());

		TRACE(std::to_string(buffer.str().size()));

		int src_size = buffer.str().size(); // image size

		ZSTD_CCtx* cctx = ZSTD_createCCtx();
		ZSTD_CCtx_setParameter(cctx, ZSTD_c_compressionLevel, 3);

		ZSTD_compressBound(src_size); 

		int dst_size = ZSTD_compressBound(src_size);
		TRACE(std::to_string(dst_size));

		int ret = ZSTD_compressCCtx(cctx, p_dst, dst_size, &buffer.str()[0], src_size, 3); //
		TRACE(std::to_string(ret));
		ZSTD_freeCCtx(cctx);
		free(p_dst);
	}

	RuntimeLayer::~RuntimeLayer()
	{
		m_Scene->OnShutdownRuntime();
	}

	void RuntimeLayer::OnUpdate()
	{
		m_Scene->OnUpdateRuntime(true);
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