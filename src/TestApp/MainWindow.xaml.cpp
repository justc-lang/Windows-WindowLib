#include "pch.h"
#include "MainWindow.xaml.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;

namespace winrt::TestApp::implementation {
MainWindow::MainWindow() {
    InitializeComponent();
    auto button = TestButton();
    button.Click({ this, &MainWindow::TestButton_Click });
}

void MainWindow::TestButton_Click(IInspectable const&, RoutedEventArgs const&) {
    StatusText().Text(L"Button clicked!");
    
    auto window = this->as<Window>();
    auto appWindow = window.AppWindow();
    if (appWindow) {
        auto size = appWindow.Size();
        appWindow.Resize({ size.Width + 50, size.Height + 50 });
    }
}
}
