#include "pch.h"
#include "ClassicWindow.h"

namespace WindowLib {

ClassicWindow::ClassicWindow() = default;

ClassicWindow::~ClassicWindow() {
    Close();
}

bool ClassicWindow::Create(const WindowConfig& config) {
    m_config = config;
    
    if (!m_classRegistered) {
        WNDCLASSEX wc = {};
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = config.hInstance ? config.hInstance : GetModuleHandle(nullptr);
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.lpszClassName = m_className.c_str();
        
        if (!RegisterClassEx(&wc)) {
            return false;
        }
        m_classRegistered = true;
    }
    
    DWORD style = WS_OVERLAPPEDWINDOW;
    if (!m_config.resizable) {
        style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    }
    
    RECT rect = {0, 0, m_config.width, m_config.height};
    AdjustWindowRect(&rect, style, FALSE);
    
    m_hwnd = CreateWindowEx(
        0,
        m_className.c_str(),
        m_config.title.c_str(),
        style,
        m_config.x, m_config.y,
        rect.right - rect.left,
        rect.bottom - rect.top,
        nullptr,
        nullptr,
        GetModuleHandle(nullptr),
        this
    );
    
    if (!m_hwnd) {
        return false;
    }
    
    if (m_config.maximized) {
        ShowWindow(m_hwnd, SW_MAXIMIZE);
    }
    
    return true;
}

void ClassicWindow::Show() {
    if (m_hwnd) {
        ShowWindow(m_hwnd, SW_SHOW);
        UpdateWindow(m_hwnd);
    }
}

void ClassicWindow::Hide() {
    if (m_hwnd) {
        ShowWindow(m_hwnd, SW_HIDE);
    }
}

void ClassicWindow::Close() {
    if (m_hwnd) {
        DestroyWindow(m_hwnd);
        m_hwnd = nullptr;
    }
    m_running = false;
}

bool ClassicWindow::IsRunning() const {
    return m_running;
}

void ClassicWindow::RunMessageLoop() {
    m_running = true;
    MSG msg = {};
    while (m_running && GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void ClassicWindow::SetTitle(const std::wstring& title) {
    if (m_hwnd) {
        SetWindowText(m_hwnd, title.c_str());
    }
}

void ClassicWindow::SetSize(int width, int height) {
    if (m_hwnd) {
        RECT rect = {0, 0, width, height};
        AdjustWindowRect(&rect, GetWindowLong(m_hwnd, GWL_STYLE), FALSE);
        SetWindowPos(m_hwnd, nullptr, 0, 0,
                     rect.right - rect.left,
                     rect.bottom - rect.top,
                     SWP_NOMOVE | SWP_NOZORDER);
    }
}

void ClassicWindow::OnResize(ResizeCallback callback) {
    m_resizeCallback = std::move(callback);
}

void ClassicWindow::OnClose(CloseCallback callback) {
    m_closeCallback = std::move(callback);
}

LRESULT CALLBACK ClassicWindow::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    ClassicWindow* pThis = nullptr;
    
    if (msg == WM_NCCREATE) {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        pThis = reinterpret_cast<ClassicWindow*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    } else {
        pThis = reinterpret_cast<ClassicWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }
    
    if (pThis) {
        return pThis->HandleMessage(msg, wParam, lParam);
    }
    
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT ClassicWindow::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_SIZE: {
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);
            if (m_resizeCallback) {
                m_resizeCallback(width, height);
            }
            return 0;
        }
        
        case WM_CLOSE: {
            if (m_closeCallback) {
                m_closeCallback();
            } else {
                DestroyWindow(m_hwnd);
            }
            return 0;
        }
        
        case WM_DESTROY: {
            m_running = false;
            PostQuitMessage(0);
            return 0;
        }
        
        default:
            return DefWindowProc(m_hwnd, msg, wParam, lParam);
    }
}

std::unique_ptr<IWindow> CreateClassicWindow() {
    return std::make_unique<ClassicWindow>();
}

}
