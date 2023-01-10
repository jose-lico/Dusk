project "DuskRuntime"
	kind "ConsoleApp"
	language "C++"

	targetdir ("%{wks.location}/bin/" .. outputDir .. "/%{prj.name}")
	objdir  ("%{wks.location}/bin-int/" .. outputDir .. "/%{prj.name}")
	targetname ("%{prj.name}-%{cfg.system}-%{cfg.buildcfg}-%{cfg.architecture}")
	
	files
	{
		"src/**.h",
		"src/**.cpp",

		-- temp
		"%{wks.location}/DuskEditor/src/DuskEditor/Assets/AssetDatabaseEditor.cpp",
		"%{wks.location}/DuskEditor/src/DuskEditor/Importers/ImageImporter.cpp",
		"%{wks.location}/DuskEditor/src/DuskEditor/Utils/stb_image.cpp",
	}


	links
	{
		"DuskEngine",
		"glfw3",
		"glew",
		"ImGui",
		"yaml-cpp",
		"ImGuizmo",
		"rttr",
		"lua",
		"zstd"
	}

	debugdir "%{wks.location}/DuskEditor"

	includedirs
	{
		"%{wks.location}/DuskEngine/src",
		"%{wks.location}/DuskEditor/src",
		dependenciesDir .. "/glfw/include",
		dependenciesDir .. "/glew/include",
		dependenciesDir .. "/glm/include",
		dependenciesDir .. "/stb/include",
		dependenciesDir .. "/spdlog/include",
		dependenciesDir .. "/imgui/include",
		dependenciesDir .. "/entt/src",
		dependenciesDir .. "/yaml-cpp/include",
		dependenciesDir .. "/stduuid/include",
		dependenciesDir .. "/stduuid/gsl",
		dependenciesDir .. "/IconFontCppHeaders",
		dependenciesDir .. "/ImGuizmo/include",
		dependenciesDir .. "/rttr/include",
		dependenciesDir .. "/lua/include/lua",
		dependenciesDir .. "/sol2/include",
		dependenciesDir .. "/zstd/lib"
	}

	defines
	{
		-- Lib includes
		"YAML_CPP_STATIC_DEFINE",
		"UUID_SYSTEM_GENERATOR",
		"NOMINMAX",

		"DUSK_EXE",
		"DUSK_IMGUI"
	}

	postbuildcommands 
	{
		"{COPY} %{wks.location}/DuskEditor/res %{cfg.targetdir}"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "on"
		systemversion "latest"
		defines "DUSK_WINDOWS"
		links "opengl32"
		entrypoint "mainCRTStartup"
		debugdir ("%{wks.location}/bin/%{cfg.targetdir}")
		
	filter "system:linux"
		cppdialect "gnu++17"
		systemversion "latest"
		defines "DUSK_LINUX"
		
		links
		{
			"GL",
			"pthread",
			"dl",
			"uuid"
		}

	filter "configurations:Debug"
		kind "ConsoleApp"
		runtime "Debug"
		symbols "On"
		defines "DUSK_DEBUG"

	filter "configurations:ReleaseWithInfo"
		kind "ConsoleApp"
		runtime "Release"
		optimize "On"
		defines "DUSK_DEBUG"
	
	filter "configurations:ReleaseNoImGui"
		kind "ConsoleApp"
		runtime "Release"
		optimize "On"
		defines "DUSK_RELEASE"
		undefines "DUSK_IMGUI"

	filter "configurations:Release"
		kind "WindowedApp"
		runtime "Release"
		optimize "On"
		defines "DUSK_RELEASE"
