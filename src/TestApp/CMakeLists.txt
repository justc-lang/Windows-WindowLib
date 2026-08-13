set(SOURCES
    Main.cpp
)

add_executable(TestApp WIN32 ${SOURCES})

target_link_libraries(TestApp
    WindowLib
    user32
    kernel32
)

target_compile_definitions(TestApp PRIVATE
    UNICODE
    _UNICODE
    WIN32_LEAN_AND_MEAN
)

target_compile_options(TestApp PRIVATE 
    /EHsc 
    /std:c++17
    /W4
)

add_custom_command(TARGET TestApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:WindowLib>
        $<TARGET_FILE_DIR:TestApp>
    COMMENT "Copying WindowLib.dll to TestApp directory"
)
