#include <windows.h>
#include <iostream>
#include <memory>
#include "../WindowLib/IWindow.h"
#include <string>

using namespace WindowLib;

void ShowError(const std::wstring& message, DWORD errorCode = 0) {
    std::wstring fullMessage = message;
    if (errorCode != 0) {
        fullMessage += L"\nError code: " + std::to_wstring(errorCode);
        
        LPWSTR errorText = nullptr;
        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
            nullptr,
            errorCode,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPWSTR)&errorText,
            0,
            nullptr
        );
        if (errorText) {
            fullMessage += L"\nDescription: ";
            fullMessage += errorText;
            LocalFree(errorText);
        }
    }
    MessageBox(nullptr, fullMessage.c_str(), L"WindowLib Error", MB_OK | MB_ICONERROR);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    OutputDebugString(L"Starting WindowLib Test Application\n");
    
    try {
        auto window = CreateClassicWindow();
        
        if (!window) {
            ShowError(L"Failed to create window instance");
            return 1;
        }
        
        OutputDebugString(L"Window instance created\n");
        
        WindowConfig config;
        config.title = L"WindowLib Test Application";
        config.width = 800;
        config.height = 600;
        config.hInstance = hInstance;
        config.resizable = true;
        config.maximized = false;
        config.x = CW_USEDEFAULT;
        config.y = CW_USEDEFAULT;
        
        OutputDebugString(L"Creating window...\n");
        
        if (!window->Create(config)) {
            ShowError(L"Failed to create window");
            return 1;
        }
        
        OutputDebugString(L"Window created successfully\n");
        
        window->OnResize([](int width, int height) {
            wchar_t buffer[256];
            swprintf_s(buffer, L"Window resized: %dx%d\n", width, height);
            OutputDebugString(buffer);
        });
        
        window->OnClose([]() {
            OutputDebugString(L"Window closing\n");
        });
        
        OutputDebugString(L"Showing window...\n");
        
        window->Show();
        
        OutputDebugString(L"Starting message loop...\n");
        
        window->RunMessageLoop();
        
        OutputDebugString(L"Application terminated normally\n");
        
    } catch (const std::exception& e) {
        std::wstring error = L"Exception: ";
        error += std::wstring(e.what(), e.what() + strlen(e.what()));
        ShowError(error);
        return 1;
    } catch (...) {
        ShowError(L"Unknown exception occurred");
        return 1;
    }
    
    return 0;
}
