workspace "Dusk"
	architecture "x64"
	outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	dependenciesDir = "%{wks.location}/Dependencies"

	configurations
	{
		"Debug",
		"Release",
		"ReleaseNoImGui"
	}

	startproject "DuskEditor"

if _ACTION ~= "gmake2" then
	group "Dependencies"
end
	include "Dependencies/glew"
	include "Dependencies/glfw"
	include "Dependencies/imgui"
	include "Dependencies/assimp-premake"
	include "Dependencies/yaml-cpp"
	include "Dependencies/ImGuizmo"
	include "Dependencies/rttr"
	include "Dependencies/lua"
if _ACTION ~= "gmake2" then
	group "Engine"
end
	include "DuskEngine"
	include "DuskEditor"
	include "DuskRuntime"

newaction
{
	trigger = "clean",
	description = "Cleans the generated projected files, binaries and intermediates",
	execute = function()
		print("Removing binaries...")
		os.rmdir("**bin")
		print("Removing intermediate files...")
		os.rmdir("**bin-int")
		print("Removing project files...")
		os.remove("**.sln")
		os.remove("**.vcxproj")
		os.remove("**.vcxproj.filters")
		os.remove("**.vcxproj.user")
		os.remove("**Makefile")
		print("Done")
		end
}