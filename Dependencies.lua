OpenCV_DIRLIB = os.getenv("OpenCV_LIB")
OpenCV_DIRINC = os.getenv("OpenCV_INC")

if not OpenCV_DIRLIB then
    print("OpenCV_DIR environment variable not set! Ensure OpenCV is installed and OpenCV_DIRLIB is set.")
    os.exit() -- Exit if OpenCV_DIR isn't set. You can handle this differently if you want.
end


IncludeDir = {}
IncludeDir["OpenCV"] = "%{OpenCV_DIRINC}"
IncludeDir["GLFW"]   = "%{wks.location}/SoccerVisualization/vendor/GLFW/include"
IncludeDir["Glad"]   = "%{wks.location}/SoccerVisualization/vendor/Glad/include"
IncludeDir["ImGui"]  = "%{wks.location}/SoccerVisualization/vendor/ImGui"

LibraryDir = {}
LibraryDir["OpenCV"] = "%{OpenCV_DIRLIB}"  -- Assuming the library files are in the 'lib' sub-directory

Library = {}
Library["OpenCV"] = "opencv_world480.lib" -- This will typically be the base name of the library without the version or file extension
