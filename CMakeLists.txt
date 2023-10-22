cmake_minimum_required(VERSION 3.26)
project(SoccerCamRecorder)

set(CMAKE_CXX_STANDARD 23)

#finding packages
find_package( OpenCV REQUIRED )

include_directories({OpenCV_INCLUDE_DIRS})

set(SOURCES main.cpp
        Utils/CudaUtils.h
        Utils/TimeUtils.h
        Utils/ImageUtils.h
        Utils/Log.h
)
add_executable(SoccerCamRecorder ${SOURCES})

target_link_libraries(SoccerCamRecorder ${OpenCV_LIBS})

# Add OpenCV include directories
target_include_directories(SoccerCamRecorder PRIVATE ${OpenCV_INCLUDE_DIRS})



