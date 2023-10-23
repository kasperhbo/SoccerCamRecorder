include(FetchContent)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 6.2.1
        SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/fmt"
)

FetchContent_GetProperties(fmt)
if(NOT fmt_POPULATED)
        FetchContent_Populate(fmt)
        add_subdirectory(${fmt_SOURCE_DIR} ${fmt_BINARY_DIR})
endif()
