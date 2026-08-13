#pragma once
#include <string>
#include <functional>
#include <windows.h>

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
    
    virtual bool Create(const WindowConfig& config) = 0;
    virtual void Show() = 0;
    virtual void Hide() = 0;
    virtual void Close() = 0;
    virtual bool IsRunning() const = 0;
    virtual void RunMessageLoop() = 0;
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
