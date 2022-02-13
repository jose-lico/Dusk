workspace "Dusk"
	architecture "x64"
	outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	dependenciesDir = "%{wks.location}/Dependencies"

	configurations
	{
		"Debug",
		"Release",
		"MinSizeRelease"
	}

	startproject "DuskEditor"

if _ACTION ~= "gmake2" then
	group "Dependencies"
end
	include "Dependencies/glew"
	include "Dependencies/glfw"
	include "Dependencies/imgui"
	include "Dependencies/assimp-1"
	include "Dependencies/nativefiledialog/build"
	include "Dependencies/yaml-cpp"
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