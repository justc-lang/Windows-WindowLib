#include "pch.h"
#include "App.xaml.h"
#include "MainWindow.xaml.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::TestApp::implementation {
void App::OnLaunched(LaunchActivatedEventArgs const&) {
    MainWindow window;
    window.Activate();
}
}
