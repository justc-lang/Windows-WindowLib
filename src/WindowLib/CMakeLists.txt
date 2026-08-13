set(SOURCES
    pch.h
    pch.cpp
    IWindow.h
    ClassicWindow.cpp
    ClassicWindow.h
    WinUIWindow.cpp
    WinUIWindow.h
)

add_library(WindowLib SHARED ${SOURCES})

target_include_directories(WindowLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(WindowLib
    user32
    kernel32
    gdi32
    ole32
    shell32
    shlwapi
    runtimeobject
)

target_compile_options(WindowLib PRIVATE 
    /EHsc 
    /std:c++17
    /W4
    /wd4251
)

target_compile_definitions(WindowLib PRIVATE
    WINRT_LEAN_AND_MEAN
    _CRT_SECURE_NO_WARNINGS
)

if(MSVC)
    target_compile_options(WindowLib PRIVATE /ZW)
endif()
