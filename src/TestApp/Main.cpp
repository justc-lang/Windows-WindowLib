#include <windows.h>
#include <iostream>
#include <memory>
#include "WindowLib/IWindow.h"

using namespace WindowLib;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    auto window = CreateClassicWindow();
    
    if (!window) {
        MessageBox(nullptr, L"Failed to create window instance", L"Error", MB_OK);
        return 1;
    }
    
    WindowConfig config;
    config.title = L"WindowLib Test Application";
    config.width = 800;
    config.height = 600;
    config.hInstance = hInstance;
    config.resizable = true;
    config.maximized = false;
    
    if (!window->Create(config)) {
        MessageBox(nullptr, L"Failed to create window", L"Error", MB_OK);
        return 1;
    }
    
    window->OnResize([](int width, int height) {
        wchar_t buffer[256];
        swprintf_s(buffer, L"Resized: %dx%d\n", width, height);
        OutputDebugString(buffer);
    });
    
    window->OnClose([]() {
        OutputDebugString(L"Window closed\n");
    });
    
    window->Show();
    
    window->RunMessageLoop();
    
    return 0;
}
