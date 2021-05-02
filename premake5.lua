workspace "DuskEngine"
	architecture "x64"
	outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	configurations
	{
		"Debug",
		"Release",
		"MinSizeRelease"
	}

	startproject "DuskEditor"

group "" -- group seems to be bugged for gmake2, so for now, no virtual folders
	include "DuskEngine"
	include "DuskEditor"

newaction
{

	trigger = "clean",
	description = "Cleans the generated projected files, binaries and intermediates",
	execute = function()
		print("Removing binaries...")
		os.rmdir("./bin")
		print("Removing intermediate files...")
		os.rmdir("./bin-int")
		print("Removing project files...")
		os.remove("**.sln")
		os.remove("**.vcxproj")
		os.remove("**.vcxproj.filters")
		os.remove("**.vcxproj.user")
		os.remove("**Makefile")
		print("Done")
		end
}