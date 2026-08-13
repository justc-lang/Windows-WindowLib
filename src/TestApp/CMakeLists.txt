# MIT License
#
# Copyright (c) 2026 JustStudio. <https://juststudio.is-a.dev/>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

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
