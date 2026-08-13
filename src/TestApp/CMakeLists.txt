set(SOURCES
    Main.cpp
)

set(XAML_FILES
    App.xaml
    MainWindow.xaml
)

add_executable(TestApp WIN32 ${SOURCES} ${XAML_FILES})

target_link_libraries(TestApp
    WindowLib
    user32
    kernel32
    ole32
    runtimeobject
    windowsapp
)

target_compile_options(TestApp PRIVATE 
    /EHsc 
    /std:c++17
    /ZW
    /W4
)

target_compile_definitions(TestApp PRIVATE
    UNICODE
    _UNICODE
    WINRT_LEAN_AND_MEAN
)

add_custom_command(TARGET TestApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:WindowLib>
        $<TARGET_FILE_DIR:TestApp>
    COMMENT "Copying WindowLib.dll to TestApp directory"
)
