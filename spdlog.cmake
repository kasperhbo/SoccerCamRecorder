include(FetchContent)

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.8.5
        SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/spdlog"
)

set(SPDLOG_FMT_EXTERNAL ON CACHE INTERNAL "Use external fmt")

FetchContent_GetProperties(spdlog)
if(NOT spdlog_POPULATED)
    FetchContent_Populate(spdlog)
    add_subdirectory(${spdlog_SOURCE_DIR} ${spdlog_BINARY_DIR})
endif()
