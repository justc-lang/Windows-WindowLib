cmake_minimum_required(VERSION 3.28)

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

if(MSVC)
    set_target_properties(WindowLib PROPERTIES
        VS_PACKAGE_REFERENCES "Microsoft.Windows.CppWinRT_2.0.240405.15;Microsoft.WindowsAppSDK_1.5.240607001"
    )

    set_target_properties(WindowLib PROPERTIES
        VS_GLOBAL_TargetFramework "native"
        VS_GLOBAL_AssetTargetFallback "native"
    )
endif()

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
    UNICODE
    _UNICODE
)

if(MSVC)
    get_filename_component(WINDOWS_SDK_DIR "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Microsoft SDKs\\Windows\\v10.0;InstallationFolder]" ABSOLUTE CACHE)
    
    if(NOT WINDOWS_SDK_DIR)
        set(WINDOWS_SDK_DIR "$ENV{WindowsSdkDir}")
    endif()
    
    if(WINDOWS_SDK_DIR)
        file(GLOB WINDOWS_SDK_VERSIONS RELATIVE "${WINDOWS_SDK_DIR}/Include/" "${WINDOWS_SDK_DIR}/Include/*")
        list(SORT WINDOWS_SDK_VERSIONS)
        list(REVERSE WINDOWS_SDK_VERSIONS)
        
        foreach(VERSION ${WINDOWS_SDK_VERSIONS})
            if(VERSION MATCHES "^[0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+$")
                set(WINDOWS_SDK_VERSION ${VERSION})
                break()
            endif()
        endforeach()
        
        if(WINDOWS_SDK_VERSION)
            set(WINDOWS_SDK_REF_PATH "${WINDOWS_SDK_DIR}/References/${WINDOWS_SDK_VERSION}")
            
            target_compile_options(WindowLib PRIVATE 
                "/AI${WINDOWS_SDK_REF_PATH}"
                "/AI${WINDOWS_SDK_DIR}/UnionMetadata/${WINDOWS_SDK_VERSION}"
            )
            
            target_link_directories(WindowLib PRIVATE
                "${WINDOWS_SDK_DIR}/Lib/${WINDOWS_SDK_VERSION}/um/x64"
                "${WINDOWS_SDK_DIR}/Lib/${WINDOWS_SDK_VERSION}/ucrt/x64"
            )
            
            message(STATUS "Windows SDK found at: ${WINDOWS_SDK_DIR}")
            message(STATUS "Windows SDK version: ${WINDOWS_SDK_VERSION}")
        endif()
    endif()
endif()
