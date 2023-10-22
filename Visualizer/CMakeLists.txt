cmake_minimum_required(VERSION 3.26)
project(Visualizer)

set(CMAKE_CXX_STANDARD 23)

#fmt
include(fmt)
include_directories("../third_party/fmt/include")

#spdlog
include(spdlog)#
include_directories("../third_party/spdlog/include/")

#finding opencv
find_package( OpenCV REQUIRED )
include_directories({OpenCV_INCLUDE_DIRS})


set(SOURCES
        VisualizerApplication.cpp
)
add_executable(Visualizer ${SOURCES})

target_link_libraries(Visualizer ${OpenCV_LIBS})

# Add OpenCV include directories
target_include_directories(Visualizer PRIVATE ${OpenCV_INCLUDE_DIRS})




