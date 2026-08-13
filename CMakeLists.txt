cmake_minimum_required(VERSION 3.20)
project(WindowLib VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_compile_definitions(
    WIN32_LEAN_AND_MEAN
    _CRT_SECURE_NO_WARNINGS
    NOMINMAX
    WINVER=0x0A00
    _WIN32_WINNT=0x0A00
)

if(MSVC)
    add_compile_options(/EHsc /bigobj)
endif()

add_subdirectory(src/WindowLib)
add_subdirectory(src/TestApp)
