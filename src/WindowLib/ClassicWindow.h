/*

MIT License

Copyright (c) 2026 JustStudio. <https://juststudio.is-a.dev/>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#pragma once
#include "IWindow.h"
#include <windows.h>
#include <string>
#include <functional>

namespace WindowLib {

class ClassicWindow : public IWindow {
public:
    ClassicWindow();
    ~ClassicWindow() override;
    
    bool Create(const WindowConfig& config, const bool& debug = false) override;
    void Show(const bool& debug = false) override;
    void Hide() override;
    void Close(const bool& debug = false) override;
    bool IsRunning() const override;
    void RunMessageLoop(const bool& debug = false) override;
    void SetTitle(const std::wstring& title) override;
    void SetSize(int width, int height) override;
    
    void OnResize(ResizeCallback callback) override;
    void OnClose(CloseCallback callback) override;
    
    HWND GetHandle() const { return m_hwnd; }

private:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam, const bool& debug = false);
    
    HWND m_hwnd = nullptr;
    bool m_running = false;
    ResizeCallback m_resizeCallback;
    CloseCallback m_closeCallback;
    WindowConfig m_config;
    std::wstring m_className;
    bool m_classRegistered = false;
};

}
