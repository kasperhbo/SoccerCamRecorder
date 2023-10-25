project "Analyzer"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

    
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    
	files
	{
		"src/**.h",
		"src/**.cpp"
	}


    includedirs
	{
		"%{wks.location}/SoccerVisualization/vendor/spdlog/include",
		"%{wks.location}/SoccerVisualization/src",
		"%{wks.location}/SoccerVisualization/vendor",
		"%{IncludeDir.OpenCV}",
	}

	libdirs
	{
		"%{LibraryDir.OpenCV}", -- Add the library directory for OpenCV here=
	}

    links
	{
		"SoccerVisualization",
		"%{Library.OpenCV}",
	}

    
    
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
