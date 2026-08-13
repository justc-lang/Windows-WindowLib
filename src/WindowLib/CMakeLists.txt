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
    windowsapp
    ole32
    shell32
    shlwapi
)

target_compile_options(WindowLib PRIVATE /EHsc /ZW /std:c++17)
