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
#include <string>
#include <functional>
#include <windows.h>
#include <memory>

namespace WindowLib {

struct WindowConfig {
    std::wstring title = L"Window";
    int width = 800;
    int height = 600;
    int x = CW_USEDEFAULT;
    int y = CW_USEDEFAULT;
    bool resizable = true;
    bool maximized = false;
    HINSTANCE hInstance = nullptr;
};

class IWindow {
public:
    virtual ~IWindow() = default;
    
    virtual bool Create(const WindowConfig& config, const bool& debug = false) = 0;
    virtual void Show(const bool& debug = false) = 0;
    virtual void Hide() = 0;
    virtual void Close(const bool& debug = false) = 0;
    virtual bool IsRunning() const = 0;
    virtual void RunMessageLoop(const bool& debug = false) = 0;
    virtual void SetTitle(const std::wstring& title) = 0;
    virtual void SetSize(int width, int height) = 0;
    
    using ResizeCallback = std::function<void(int, int)>;
    using CloseCallback = std::function<void()>;
    
    virtual void OnResize(ResizeCallback callback) = 0;
    virtual void OnClose(CloseCallback callback) = 0;
};

std::unique_ptr<IWindow> CreateClassicWindow();
std::unique_ptr<IWindow> CreateWinUIWindow();

}
