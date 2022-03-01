project "DuskEditor"
	kind "ConsoleApp"
	language "C++"

	targetdir ("%{wks.location}/bin/" .. outputDir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputDir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	links
	{
		"DuskEngine",
		"nfd",
		"glfw3",
		"glew",
		"ImGui",
		"assimp",
		"yaml-cpp",
		"ImGuizmo"
	}

	includedirs
	{
		"%{wks.location}/DuskEngine/src",
		dependenciesDir .. "/glfw/include",
		dependenciesDir .. "/glew/include",
		dependenciesDir .. "/glm/include",
		dependenciesDir .. "/spdlog/include",
		dependenciesDir .. "/imgui/include",
		dependenciesDir .. "/entt/src",
		dependenciesDir .. "/nativefiledialog/src/include",
		dependenciesDir .. "/assimp-1/include",
		dependenciesDir .. "/yaml-cpp/include",
		dependenciesDir .. "/visit_struct/include",
		dependenciesDir .. "/stduuid/include",
		dependenciesDir .. "/stduuid/gsl",
		dependenciesDir .. "/IconFontCppHeaders",
		dependenciesDir .. "/ImGuizmo/include"
	}

	defines
	{
		"DUSK_DEBUG",
		"DUSK_EXE"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "on"
		systemversion "latest"
		defines "DUSK_WINDOWS"

	filter "system:linux"
		cppdialect "gnu++17"
		systemversion "latest"
		defines "DUSK_LINUX"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"
	
	filter "configurations:Release"
		runtime "Release"
		optimize "On"

	filter "configurations:MinSizeRelease"
		runtime "Release"
		optimize "On"