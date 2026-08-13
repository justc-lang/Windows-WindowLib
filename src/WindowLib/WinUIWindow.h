#pragma once
#include "IWindow.h"
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Window.h>

namespace WindowLib {

class WinUIWindow : public IWindow {
public:
    WinUIWindow();
    ~WinUIWindow() override;
    
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

private:
    winrt::Microsoft::UI::Xaml::Window m_window{ nullptr };
    winrt::Microsoft::UI::Xaml::Controls::Grid m_rootGrid{ nullptr };
    bool m_running = false;
    ResizeCallback m_resizeCallback;
    CloseCallback m_closeCallback;
    WindowConfig m_config;
};

}
