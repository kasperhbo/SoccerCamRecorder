cmake_minimum_required(VERSION 3.20)
project(SoccerCamRecorder)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")

# Include the subprojects.
add_subdirectory(Recorder)
add_subdirectory(Visualizer)
add_subdirectory(SoccerVisualization)
