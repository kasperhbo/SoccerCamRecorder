project "SoccerVisualization"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "scrpch.h"
	pchsource "src/scrpch.cpp"

    files
	{
		"src/**.h",
		"src/**.cpp"
	}

    defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

    includedirs
	{
		"src",
        "vendor/fmt/include",
		"vendor/spdlog/include",
		"%{IncludeDir.OpenCV}"
	}


    libdirs
	{
		"%{LibraryDir.OpenCV}" -- Add the library directory for OpenCV here
	}

    links
	{
        "%{Library.OpenCV}",
	}
    
    flags { "NoPCH" }

    filter "system:windows"
        systemversion "latest"

        defines
        {
        }

        links
        {

        }

    filter "configurations:Release"
        defines "SoccerVisualization_RELEASE"
        runtime "Release"
        optimize "on"