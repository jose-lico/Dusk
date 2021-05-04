project "DuskEngine"
	kind "SharedLib"
	language "C++"

	targetdir ("%{wks.location}/bin/" .. outputDir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputDir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",
	}

	includedirs
	{
		"src",
		dependenciesDir .. "/glfw/include",
		dependenciesDir .. "/glew/include"
	}

	links
	{
		"glfw3",
		"glew"
	}

	defines
	{
		"GLFW_INCLUDE_NONE",
		"GLEW_STATIC"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "off"
		systemversion "latest"
		defines 
		{
			"DUSK_WINDOWS",
			"DUSK_DLL"
		}
		links
		{
			"opengl32"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputDir .. "/DuskEditor")
		}

	filter "system:linux"
		cppdialect "gnu++17"
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
