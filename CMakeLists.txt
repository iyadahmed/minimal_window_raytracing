cmake_minimum_required(VERSION 3.16)
project(minimal_window_raytracing_example)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED YES)
set(CMAKE_C_EXTENSIONS NO)

set(CPM_SOURCE_CACHE ${CMAKE_CURRENT_SOURCE_DIR}/.cpm_cache)
include(cmake/CPM.cmake)

add_executable(minimal_window_raytracing_example
    main.c
)
find_library(LIB_MATH m)

CPMAddPackage(
    NAME minimal_window
    GITHUB_REPOSITORY iyadahmed/minimal_window
    GIT_TAG d8ad68a06f4812602d9fa785a59b0fdb255a7c76
    OPTIONS

    # Uncomment to force X11 on Linux
    # "FORCE_X11_ON_LINUX ON"
)
target_link_libraries(minimal_window_raytracing_example PRIVATE minimal_window)

if(LIB_MATH)
    target_link_libraries(minimal_window_raytracing_example PUBLIC ${LIB_MATH})
endif()
