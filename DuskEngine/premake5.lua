project "DuskEngine"
	kind "SharedLib"
	language "C++"

	targetdir ("%{wks.location}/bin/" .. outputDir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputDir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "src/pch.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp",
		dependenciesDir .. "/stb/stb_image.h",
		dependenciesDir .. "/stb/stb_image.cpp"
	}

	includedirs
	{
		"src",
		dependenciesDir .. "/glfw/include",
		dependenciesDir .. "/glew/include",
		dependenciesDir .. "/glm",
		dependenciesDir .. "/stb",
		dependenciesDir .. "/imgui",
		dependenciesDir .. "/spdlog/include",
		dependenciesDir .. "/entt/src"
	}

	links
	{
		"glfw3",
		"glew",
		"ImGui"
	}

	defines
	{
		"GLFW_INCLUDE_NONE",
		"GLEW_STATIC",
		"_CRT_SECURE_NO_WARNINGS",

		"DUSK_OPENGL",
		"DUSK_DEBUG",
		"DUSK_DLL"
	}


	filter "system:windows"
		cppdialect "C++17"
		staticruntime "off"
		systemversion "latest"

		defines 
		{
			"DUSK_WINDOWS"
		}
		links
		{
			"opengl32"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputDir .. "/DuskEditor")
		}

		disablewarnings 
		{ 
			"4005" 
		}

	filter "system:linux"
		cppdialect "C++17"
		systemversion "latest"
		defines "DUSK_LINUX"
		links 
		{
			"GL",
			"pthread"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"
	
	filter "configurations:Release"
		runtime "Release"
		optimize "On"

	filter "configurations:MinSizeRelease"
		runtime "Release"
		optimize "On"
