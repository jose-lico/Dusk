project "DuskEditor"
	kind "ConsoleApp"
	language "C++"
	configmap {
        ["ReleaseNoImGui"] = "Release"
    }

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
		"glfw3",
		"glew",
		"ImGui",
		"assimp",
		"yaml-cpp",
		"ImGuizmo",
		"rttr",
		"lua"
	}

	debugdir "%{wks.location}/DuskEditor"

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
		dependenciesDir .. "/assimp-premake/include",
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
		"DUSK_EXE",
		"DUSK_IMGUI"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "on"
		systemversion "latest"
		defines "DUSK_WINDOWS"
		links "opengl32"

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
		runtime "Debug"
		symbols "On"
		defines "DUSK_DEBUG"
	
	filter "configurations:Release"
		runtime "Release"
		optimize "On"
