cmake_minimum_required(VERSION 3.16)
project(NanoGUI_raytracing_example)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED YES)
set(CMAKE_C_EXTENSIONS NO)

add_subdirectory(NanoGUI)

add_executable(NanoGUI_raytracing_example
    main.c
)
find_library(LIB_MATH m)
target_link_libraries(NanoGUI_raytracing_example PRIVATE NanoGUI ${LIB_MATH})
