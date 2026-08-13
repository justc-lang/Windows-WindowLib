#include "pch.h"
#include "WinUIWindow.h"
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Windowing.h>

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;
using namespace Microsoft::UI::Xaml::Media;

namespace WindowLib {

WinUIWindow::WinUIWindow() {
    // Инициализация WinRT
    winrt::init_apartment();
}

WinUIWindow::~WinUIWindow() {
    Close();
}

bool WinUIWindow::Create(const WindowConfig& config) {
    m_config = config;
    
    try {
        m_window = Window();
        m_window.Title(to_hstring(config.title));
        
        m_rootGrid = Grid();
        m_rootGrid.Background(SolidColorBrush(Colors::White()));
        m_window.Content(m_rootGrid);
        
        if (config.width > 0 && config.height > 0) {
            auto appWindow = m_window.AppWindow();
            if (appWindow) {
                appWindow.Resize({ config.width, config.height });
            }
        }
        
        return true;
    } catch (...) {
        return false;
    }
}

void WinUIWindow::Show() {
    if (m_window) {
        m_window.Activate();
        m_running = true;
    }
}

void WinUIWindow::Hide() {
    if (m_window) {
        m_window.Activate();
    }
}

void WinUIWindow::Close() {
    if (m_window) {
        m_window.Close();
        m_window = nullptr;
    }
    m_running = false;
}

bool WinUIWindow::IsRunning() const {
    return m_running;
}

void WinUIWindow::RunMessageLoop() {
    m_running = true;
    winrt::Windows::UI::Core::CoreWindow coreWindow = 
        m_window.as<winrt::Windows::UI::Core::ICoreWindow>();
    
    while (m_running) {
        winrt::Windows::UI::Core::CoreWindow::GetForCurrentThread().Dispatcher().ProcessEvents(
            winrt::Windows::UI::Core::CoreProcessEventsOption::ProcessAllIfPresent
        );
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void WinUIWindow::SetTitle(const std::wstring& title) {
    if (m_window) {
        m_window.Title(to_hstring(title));
    }
}

void WinUIWindow::SetSize(int width, int height) {
    if (m_window) {
        auto appWindow = m_window.AppWindow();
        if (appWindow) {
            appWindow.Resize({ width, height });
        }
    }
}

void WinUIWindow::OnResize(ResizeCallback callback) {
    m_resizeCallback = std::move(callback);
}

void WinUIWindow::OnClose(CloseCallback callback) {
    m_closeCallback = std::move(callback);
    if (m_window) {
        m_window.Closed([this](auto&&, auto&&) {
            if (m_closeCallback) {
                m_closeCallback();
            }
            m_running = false;
        });
    }
}

std::unique_ptr<IWindow> CreateWinUIWindow() {
    return std::make_unique<WinUIWindow>();
}

}
