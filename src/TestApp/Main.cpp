#include <windows.h>
#include <iostream>
#include "WindowLib/IWindow.h"
#include "App.xaml.h"

using namespace WindowLib;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    bool useWinUI = true;
    
    std::unique_ptr<IWindow> window;
    
    if (useWinUI) {
        window = CreateWinUIWindow();
    } else {
        window = CreateClassicWindow();
    }
    
    if (!window) {
        MessageBox(nullptr, L"Failed to create window", L"Error", MB_OK);
        return 1;
    }
    
    WindowConfig config;
    config.title = L"WindowLib Test Application";
    config.width = 800;
    config.height = 600;
    config.hInstance = hInstance;
    
    if (!window->Create(config)) {
        MessageBox(nullptr, L"Failed to create window", L"Error", MB_OK);
        return 1;
    }
    
    window->OnResize([](int width, int height) {
        OutputDebugString((L"Resized: " + std::to_wstring(width) + 
                          L"x" + std::to_wstring(height) + L"\n").c_str());
    });
    
    window->OnClose([]() {
        OutputDebugString(L"Window closed\n");
    });
    
    window->Show();
    
    window->RunMessageLoop();
    
    return 0;
}
