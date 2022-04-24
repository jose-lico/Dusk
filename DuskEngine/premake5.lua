project "DuskEngine"
	kind "StaticLib"
	language "C++"

	targetdir ("%{wks.location}/bin/" .. outputDir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputDir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "src/pch.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp",
		
		dependenciesDir .. "/stb/stb/stb_image.h"
	}

	includedirs
	{
		"src",
		dependenciesDir .. "/glfw/include",
		dependenciesDir .. "/glew/include",
		dependenciesDir .. "/glm/include",
		dependenciesDir .. "/stb/include",
		dependenciesDir .. "/imgui/include",
		dependenciesDir .. "/spdlog/include",
		dependenciesDir .. "/entt/src",
		dependenciesDir .. "/assimp-premake/include",
		dependenciesDir .. "/yaml-cpp/include",
		dependenciesDir .. "/stduuid/include",
		dependenciesDir .. "/stduuid/gsl",
		dependenciesDir .. "/IconFontCppHeaders",
		dependenciesDir .. "/ImGuizmo/include",
		dependenciesDir .. "/rttr/include",
		dependenciesDir .. "/lua/include"
	}

	defines
	{
		"DUSK_OPENGL",
		"DUSK_DEBUG",
		"DUSK_LIB",
		--"DUSK_DLL",

		"GLFW_INCLUDE_NONE",
		"GLEW_STATIC",
		"_CRT_SECURE_NO_WARNINGS",
		"YAML_CPP_STATIC_DEFINE",
		"UUID_SYSTEM_GENERATOR",
		"NOMINMAX"
	}

	filter "files:../Dependencies/ImGuizmo/**.cpp"
	flags { "NoPCH" }

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "on"
		systemversion "latest"
		defines "DUSK_WINDOWS"

		disablewarnings 
		{ 
			"4003",
			"4005"
		}

	filter "system:linux"
		cppdialect "C++17"
		systemversion "latest"
		defines "DUSK_LINUX"
		links 
		{
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
