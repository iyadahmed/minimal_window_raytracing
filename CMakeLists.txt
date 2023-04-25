cmake_minimum_required(VERSION 3.16)
project(minimal_window_raytracing_example)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED YES)
set(CMAKE_C_EXTENSIONS NO)

add_subdirectory(minimal_window)

add_executable(minimal_window_raytracing_example
    main.c
)
find_library(LIB_MATH m)
target_link_libraries(minimal_window_raytracing_example PRIVATE minimal_window)

if(LIB_MATH)
    target_link_libraries(minimal_window_raytracing_example PUBLIC ${LIB_MATH})
endif()
