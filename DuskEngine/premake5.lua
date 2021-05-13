project "DuskEngine"
	kind "SharedLib"
	language "C++"

	targetdir ("%{wks.location}/bin/" .. outputDir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputDir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",
		dependenciesDir .. "/glm/glm/**.hpp",
		dependenciesDir .. "/glm/glm/**.inl",
		dependenciesDir .. "/stb/stb_image.h",
		dependenciesDir .. "/stb/stb_image.cpp"
	}

	includedirs
	{
		"src",
		dependenciesDir .. "/glfw/include",
		dependenciesDir .. "/glew/include",
		dependenciesDir .. "/glm/glm",
		dependenciesDir .. "/stb",
		dependenciesDir .. "/imgui"
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
		"IMGUI_IMPL_OPENGL_LOADER_GLEW"
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

		-- postbuildcommands
		-- {
		-- 	("{COPY} %{cfg.buildtarget.relpath} \" ../bin/" .. outputDir .. "/DuskEditor/\"")
		-- }

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
