#pragma once
#include "MainWindow.g.h"

namespace winrt::TestApp::implementation {
struct MainWindow : MainWindowT<MainWindow> {
    MainWindow();
    void TestButton_Click(winrt::Windows::Foundation::IInspectable const&, 
                          winrt::Microsoft::UI::Xaml::RoutedEventArgs const&);
};
}

namespace winrt::TestApp::factory_implementation {
struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow> {};
}
