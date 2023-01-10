project "DuskEditor"
	language "C++"
	configmap 
	{
        ["ReleaseNoImGui"] = "Release"
    }

	targetdir ("%{wks.location}/bin/" .. outputDir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputDir .. "/%{prj.name}")
	targetname ("%{prj.name}-%{cfg.system}-%{cfg.buildcfg}-%{cfg.architecture}")

	files
	{
		"src/**.h",
		"src/**.cpp"
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
		"src",
		"embedded",
		"%{wks.location}/DuskEngine/src",
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
		dependenciesDir .. "/sol2/include"
	}

	defines
	{
		-- Lib includes
		"YAML_CPP_STATIC_DEFINE",
		"UUID_SYSTEM_GENERATOR",
		"NOMINMAX",

		-- Dusk includes
		"DUSK_EXE",
		"DUSK_IMGUI"
	}

	prebuildcommands   
	{
		"python ../scripts/embed_assets.py"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "on"
		systemversion "latest"
		defines "DUSK_WINDOWS"
		links "opengl32"
		entrypoint "mainCRTStartup"

	filter "system:linux"
		kind "ConsoleApp"
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
		buildoptions "-fpermissive"

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
	
	filter "configurations:Release"
		kind "WindowedApp"
		runtime "Release"
		optimize "On"
		defines "DUSK_RELEASE"
