#include "pch.h"
#include "ClassicWindow.h"
#include <windowsx.h>
#include <string>
#include <sstream>

namespace WindowLib {

ClassicWindow::ClassicWindow() = default;

ClassicWindow::~ClassicWindow() {
    Close();
}

bool ClassicWindow::Create(const WindowConfig& config) {
    m_config = config;
    
    HINSTANCE hInstance = config.hInstance ? config.hInstance : GetModuleHandle(nullptr);
    
    if (!m_classRegistered) {
        wchar_t className[64];
        swprintf_s(className, L"ClassicWindow_%p", this);
        m_className = className;
        
        WNDCLASSEX wc = {};
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = hInstance;
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.lpszClassName = m_className.c_str();
        
        ATOM atom = RegisterClassEx(&wc);
        if (!atom) {
            DWORD error = GetLastError();
            if (error != ERROR_CLASS_ALREADY_EXISTS) {
                return false;
            }
        }
        m_classRegistered = true;
    }
    
    DWORD style = WS_OVERLAPPEDWINDOW;
    if (!m_config.resizable) {
        style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    }
    
    RECT rect = {0, 0, m_config.width, m_config.height};
    AdjustWindowRect(&rect, style, FALSE);
    
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;
    
    m_hwnd = CreateWindowEx(
        0,
        m_className.c_str(),
        m_config.title.c_str(),
        style,
        m_config.x, m_config.y,
        width, height,
        nullptr,
        nullptr,
        hInstance,
        this
    );
    
    if (!m_hwnd) {
        DWORD error = GetLastError();
        wchar_t msg[256];
        swprintf_s(msg, L"CreateWindowEx failed. Error: %d", error);
        OutputDebugString(msg);
        return false;
    }
    
    if (!IsWindow(m_hwnd)) {
        OutputDebugString(L"Window created but IsWindow returns false!");
        return false;
    }
    
    SetWindowLongPtr(m_hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
    
    if (m_config.maximized) {
        ShowWindow(m_hwnd, SW_MAXIMIZE);
    }
    
    OutputDebugString(L"Window created successfully\n");
    return true;
}

void ClassicWindow::Show() {
    if (m_hwnd && IsWindow(m_hwnd)) {
        ShowWindow(m_hwnd, SW_SHOW);
        UpdateWindow(m_hwnd);
        m_running = true;
        OutputDebugString(L"Window shown\n");
    } else {
        OutputDebugString(L"Show() called with invalid window handle\n");
    }
}

void ClassicWindow::Hide() {
    if (m_hwnd && IsWindow(m_hwnd)) {
        ShowWindow(m_hwnd, SW_HIDE);
    }
}

void ClassicWindow::Close() {
    if (m_hwnd && IsWindow(m_hwnd)) {
        OutputDebugString(L"Closing window\n");
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
    
    OutputDebugString(L"Starting message loop\n");
    
    while (m_running) {
        BOOL result = GetMessage(&msg, nullptr, 0, 0);
        
        if (result == -1) {
            OutputDebugString(L"GetMessage error\n");
            break;
        }
        
        if (result == 0) {
            OutputDebugString(L"WM_QUIT received\n");
            m_running = false;
            break;
        }
        
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    OutputDebugString(L"Message loop ended\n");
}

void ClassicWindow::SetTitle(const std::wstring& title) {
    if (m_hwnd && IsWindow(m_hwnd)) {
        SetWindowText(m_hwnd, title.c_str());
    }
}

void ClassicWindow::SetSize(int width, int height) {
    if (m_hwnd && IsWindow(m_hwnd)) {
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
        if (pThis) {
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
            pThis->m_hwnd = hwnd;
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    
    pThis = reinterpret_cast<ClassicWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    
    if (pThis && pThis->m_hwnd == hwnd) {
        return pThis->HandleMessage(msg, wParam, lParam);
    }
    
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT ClassicWindow::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            OutputDebugString(L"WM_CREATE received\n");
            return 0;
        }
        
        case WM_SIZE: {
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);
            if (m_resizeCallback) {
                m_resizeCallback(width, height);
            }
            return 0;
        }
        
        case WM_CLOSE: {
            OutputDebugString(L"WM_CLOSE received\n");
            if (m_closeCallback) {
                m_closeCallback();
            }
            DestroyWindow(m_hwnd);
            return 0;
        }
        
        case WM_DESTROY: {
            OutputDebugString(L"WM_DESTROY received\n");
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
