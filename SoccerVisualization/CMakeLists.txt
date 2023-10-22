cmake_minimum_required(VERSION 3.26)
project(SoccerCamRecorder)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")

#fmt
include(fmt)
include_directories("third_party/fmt/include")

#spdlog
include(spdlog)
include_directories("third_party/spdlog/include/")

#finding opencv
find_package( OpenCV REQUIRED )
include_directories({OpenCV_INCLUDE_DIRS})


set(SOURCES
        EntryPoint.cpp
        Utils/CudaUtils.h
        Utils/TimeUtils.h
        Utils/ImageUtils.h
        Utils/Log.h
        Base.h
        Utils/Log.cpp
        Application.cpp
        Application.h
)
add_executable(SoccerCamRecorder ${SOURCES})

target_link_libraries(SoccerCamRecorder ${OpenCV_LIBS})

# Add OpenCV include directories
target_include_directories(SoccerCamRecorder PRIVATE ${OpenCV_INCLUDE_DIRS})



