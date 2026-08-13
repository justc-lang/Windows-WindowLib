set(SOURCES
    Main.cpp
    App.xaml.cpp
    App.xaml.h
    MainWindow.xaml.cpp
    MainWindow.xaml.h
)

set(XAML_FILES
    App.xaml
    MainWindow.xaml
)

foreach(XAML_FILE ${XAML_FILES})
    set_source_files_properties(${XAML_FILE} PROPERTIES
        VS_XAML_TYPE "ApplicationDefinition"
    )
endforeach()

add_executable(TestApp WIN32 ${SOURCES} ${XAML_FILES})

target_link_libraries(TestApp
    WindowLib
    windowsapp
    ole32
    runtimeobject
)

target_compile_options(TestApp PRIVATE /EHsc /ZW /std:c++17)
target_compile_definitions(TestApp PRIVATE UNICODE _UNICODE)

add_custom_command(TARGET TestApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:WindowLib>
        $<TARGET_FILE_DIR:TestApp>
)
