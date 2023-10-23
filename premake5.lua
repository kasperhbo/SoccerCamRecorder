include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

workspace "SoccerVisualization"
	architecture "x86_64"
	startproject "Recorder"

	configurations
	{
		"Release",
	}

	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "vendor/premake"
group ""

group "Core"
	include "SoccerVisualization"
group ""

-- group "Tools"
-- 	include "Hazelnut"
-- group ""

group "Misc"
	include "Recorder"
group ""
