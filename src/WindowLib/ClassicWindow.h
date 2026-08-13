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
    
    bool Create(const WindowConfig& config) override;
    void Show() override;
    void Hide() override;
    void Close() override;
    bool IsRunning() const override;
    void RunMessageLoop() override;
    void SetTitle(const std::wstring& title) override;
    void SetSize(int width, int height) override;
    
    void OnResize(ResizeCallback callback) override;
    void OnClose(CloseCallback callback) override;
    
    HWND GetHandle() const { return m_hwnd; }

private:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);
    
    HWND m_hwnd = nullptr;
    bool m_running = false;
    ResizeCallback m_resizeCallback;
    CloseCallback m_closeCallback;
    WindowConfig m_config;
    std::wstring m_className = L"ClassicWindowClass";
    bool m_classRegistered = false;
};

}
